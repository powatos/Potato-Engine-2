/** @file IOController.cpp */

// #include <curses.h>

#include <clocale>
#include <signal.h>
#include <string>
#include <cmath>
#include <chrono>

#include "Debug/Log.hpp"
#include "Util/Vector2.hpp"
#include "Game/World.hpp"
#include "Util/GameplayHelper.hpp"
#include "Core/GameInstance.hpp"
#include "Game/PlayerController.hpp"
#include "Game/Player.hpp"
#include "Game/Camera.hpp"
#include "Core/Input.hpp"
#include "UI/Widget.hpp"
#include "UI/TextElement.hpp"
#include "UIController.hpp"

#include "IOController.hpp"

static constexpr char toggleCase(char c);
void crashHandler(int);
static constexpr Keycode GetKeycode(int ch);

IOController::IOController() : FRAMES_PER_SECOND(30.f) {
    LOG(LogType::VITAL, "IOController constructed");

    ActiveKey = Keycode::UNKNOWN;
    ImpulseKey = Keycode::UNKNOWN;

    /// Initialize Screen

    // disables escape delay (shorten if arrow/func keys not working)
#if defined(_WIN32) || defined(_WIN64)
    _putenv_s("ESCDELAY", "25");
#else
    setenv("ESCDELAY", "25", 1);
#endif

    std::setlocale(LC_ALL, "");

 //    initscr(); // init ncurses
 //    cbreak(); // disable line buffering
	// noecho(); // disable input feedback

    // WINDOW* dispWindow = newwin(24, 80, 1, 1);
    // WINDOW* boxWindow = newwin(24+2, 80+2, 0, 0);

 //    DisplayWindow = dispWindow;
 //    BoxWindow = boxWindow;
 //
	// keypad(dispWindow, TRUE); // enable keypad input
 //    nodelay(dispWindow, TRUE); // disable input delay
	// curs_set(0); // disable cursor visibility
	// wtimeout(dispWindow, 0); // Make getch() non-blocking

    // crash handlers
    signal(SIGINT, crashHandler);
    signal(SIGSEGV, crashHandler);
    
    LOG(LogType::INFO, "ncurses window initialized");
    
    // window border
    // box(boxWindow, 0, 0);
    // wrefresh(boxWindow);

}

void IOController::BeginPlay() {

    GameInstance* instance = GameInstance::Get();

    FRAMES_PER_SECOND = instance->FRAMES_PER_SECOND;
    MS_REPEAT_THRESHOLD = instance->MS_REPEAT_THRESHOLD;

}

void IOController::HandleInput() {
    static auto lastValidInput = std::chrono::steady_clock::now();

    // WINDOW* displayWindow = static_cast<WINDOW*>(this->DisplayWindow);

    // Flush input buffer to ignore old frame inputs
    int _ch = 0;
    int _lch = 0;
    // while ((_ch = wgetch(displayWindow)) != ERR) {
    //     _lch = _ch;
    // }

    const Keycode key = GetKeycode(_lch);

    if (InputBindingsImpulse.find(key) != InputBindingsImpulse.end()) {
        ImpulseKey = key;
        return;
    }

    if (MS_REPEAT_THRESHOLD == 0) {

        ActiveKey = Keycode::UNKNOWN;
        FireBinding(InputBindingsTriggered, key);

    } else {

        const auto now = std::chrono::steady_clock::now();

        if (key != Keycode::UNKNOWN) {
            // key pressed

            if (ActiveKey != key) {

                if (ActiveKey != Keycode::UNKNOWN) {
                    FireBinding(InputBindingsCompleted, ActiveKey);
                }

                ActiveKey = key;
                FireBinding(InputBindingsTriggered, ActiveKey);
            }
            
            lastValidInput = now;

        } else {
            // maybe key released, maybe mid repeat
            const auto unkownInputElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastValidInput);

            if (unkownInputElapsed > std::chrono::milliseconds(MS_REPEAT_THRESHOLD)) {
                // input completed

                const Keycode k = ActiveKey;
                ActiveKey = Keycode::UNKNOWN;

                if (k != Keycode::UNKNOWN) {
                    FireBinding(InputBindingsCompleted, k);
                }

            }
        }
    }

}

void IOController::Draw() {

    DrawLevel();
    DrawHUD();

    // doupdate();
}

void IOController::DrawLevel() {
    // WINDOW* displayWindow = static_cast<WINDOW*>(DisplayWindow);
    //
    // werase(displayWindow);
    //
    const GameInstance* Instance = GameInstance::Get();
    const World* world = Instance->GetWorld();
    const ActorPool& renderActors = world->GetAllActors();

    int maxRow = world->Settings.Size.y;
    int maxCol = world->Settings.Size.x;

    Camera* camera = Instance->GetPlayerController()->GetCamera();

    for ( Actor* actor : renderActors ) {
        if (actor == nullptr) {
            LOG(LogType::WARNING, "nullptr actor found in actor pool while drawing - skipped actor");
            continue; 
        }

        if (!actor->isVisible()) { continue; }

        const Vector2 actorPos = actor->GetPosition();
        const float rot = actor->GetTexture().GetRotation();

        if (!GameplayHelper::IsActorOverlapping(actor, camera)) { continue; }

        const Vector2 camOffsetActorPos = Vector2(actorPos.x - camera->GetPosition().x, actorPos.y);
        const Vector2 screenVector = GameplayHelper::VecToScreenVec(camOffsetActorPos);
        
        const int scrVecX = static_cast<int>(screenVector.x);
        const int scrVecY = static_cast<int>(screenVector.y);

        const float sine = std::sin(rot * PI/180.0);
        const float cosine = std::cos(rot * PI/180.0);

        auto getRotationCoords = [&scrVecX, &scrVecY, &sine, &cosine](int r, int c, int& rotRow, int& rotCol) -> void {
            const Vector2 rotVec{
                r * cosine - c * sine,
                r * sine + c * cosine
            };
            rotRow = static_cast<int>(scrVecX + rotVec.x + 0.5f);
            rotCol = static_cast<int>(scrVecY + rotVec.y + 0.5f);
        };

        if (actor->IsUsingCTex()) {
            // single-char texture
            const Vector2 actorSize = actor->GetSize();

            for (int r = 0; r < actorSize.y && r < maxRow; ++r) {
                for (int c = 0; c < actorSize.x && c < maxCol; ++c) {
                    int rotRowC;
                    int rotColC;
                    getRotationCoords(r, c, rotRowC, rotColC);

                    // mvwaddch(displayWindow,
                    //     rotRowC,
                    //     rotColC,
                    //     actor->ctex
                    // );
                }
            }

        } else {
            // regular texture
            const std::vector<std::wstring>& texVec = actor->GetTexture().raw_vec();
            int r = 0;

            for (const std::wstring& wline : texVec) {
                int c = 0;

                for (const wchar_t& ch : wline) {
                    int rotRowC;
                    int rotColC;
                    getRotationCoords(r, c, rotRowC, rotColC);

                    wchar_t wch = ch;
                    // cchar_t cc;
                    // setcchar(&cc, &wch, A_NORMAL, 0, NULL);


                    if (rotRowC > maxRow || rotColC > maxCol) { continue; }
                    
                    // mvwadd_wch(displayWindow,
                    //     rotRowC,
                    //     rotColC,
                    //     &cc
                    // );
                    ++c;
                }
                ++r;
            }

        }

    }

    // touchwin(displayWindow);
    // wnoutrefresh(displayWindow);
}

void IOController::DrawHUD() {
    using namespace std::string_literals;

    // iterate through each widget
    for (auto& [UID, map] : WidgetMaps) {
        // WINDOW* hudWindow = static_cast<WINDOW*>(map->window);
        //
        // werase(hudWindow);

        if (!map->widget->isVisible()) { continue; }
    
        // box(hudWindow, 0, 0);
        
        // iterate through each ui element on the widget
        for (auto [name, elem] : map->widget->GetAllElements()) {
            if (!elem->isVisible()) { continue; }

            const char* t  = elem->TYPE();

            if (t == "TextElement"s) {
                const TextElement* e = dynamic_cast<const TextElement*>(elem);
                const Vector2 pos = e->GetScreenPosition() + Vector2(1.f,1.f);
                // mvwprintw(hudWindow,
                //     static_cast<int>(pos.y),
                //     static_cast<int>(pos.x),
                //     "%s",
                //     e->field.c_str()
                // );
            }
            
        }
        // touchwin(hudWindow);
        // wnoutrefresh(hudWindow);
    }

}

void IOController::_TickInput(float dt) {
    HandleInput();

    FireBinding(InputBindingsOngoing, ActiveKey);
    FireBinding(InputBindingsImpulse, ImpulseKey);
    ImpulseKey = Keycode::UNKNOWN;
    
}
void IOController::_TickRender(float dt) {
    Draw();
}

#pragma region Binding management

void IOController::FireBinding(BindingMap& map, Keycode key) {
    const auto loc = map.find(key);
    if (loc == map.end()) { return; } // no binding yet

    for (const InputBinding& binding : loc->second ) {
        if (!binding.GetDelegate().Fire()) {
            LOG(LogType::WARNING, "Input event could not fire for binding: {}", binding.name);
        }
    }
}

void IOController::RegisterInputBinding(InputBinding binding) {

    switch (binding.type) {
        case InputType::Triggered:
            InputBindingsTriggered[binding.key].push_back( binding );
            break;
        case InputType::Completed:
            InputBindingsCompleted[binding.key].push_back( binding );
            break;
        case InputType::Ongoing:
            InputBindingsOngoing[binding.key].push_back( binding );
            break;
        case InputType::Impulse:
            InputBindingsImpulse[binding.key].push_back( binding );
            break;
    }

}
void IOController::RegisterInputBinding(std::initializer_list<InputBinding> bindings) {
    for (InputBinding binding : bindings) { RegisterInputBinding(binding); }
}
void IOController::UnregisterBindingFrom(BindingMap& map, std::string deleteName) {

    for (auto it = map.begin(); it != map.end(); ) {
        auto& vec = it->second;

        vec.erase(
            std::remove_if(vec.begin(), vec.end(), [deleteName](const InputBinding& binding) {
                return binding.name == deleteName;
            }),
            vec.end()
        );

        if (vec.empty()) {
            it = map.erase(it);
        } else {
            ++it;
        }

    }

}
void IOController::UnregisterAllBindingsFrom(BindingMap& map, void* object) {

    for (auto it = map.begin(); it != map.end(); ) {
        auto& vec = it->second;

        vec.erase(
            std::remove_if(vec.begin(), vec.end(), [object](const InputBinding& binding) {
                return binding.GetDelegate().GetInstance() == object;
            }),

            vec.end()
        );

        if (vec.empty()) {
            it = map.erase(it);
        } else {
            ++it;
        }

    }

}
void IOController::UnregisterInputBinding(std::string deleteName) {

    UnregisterBindingFrom(InputBindingsTriggered, deleteName);
    UnregisterBindingFrom(InputBindingsCompleted, deleteName);
    UnregisterBindingFrom(InputBindingsOngoing, deleteName);

}
void IOController::UnregisterAllInputBindings(void* object) {

    UnregisterAllBindingsFrom(InputBindingsTriggered, object);
    UnregisterAllBindingsFrom(InputBindingsCompleted, object);
    UnregisterAllBindingsFrom(InputBindingsOngoing, object);

}

#pragma endregion

#pragma region Widget management

void IOController::RegisterWidget(Widget* widget) {
    if (DisplayWindow == nullptr) { return; }
    // WINDOW* displayWindow = static_cast<WINDOW*>(DisplayWindow);

    const Vector2 widgetSize = widget->GetScreenSize();
    const Vector2 widgetPos = widget->GetScreenPosition();

    // WINDOW* win = derwin(displayWindow,
    //     static_cast<int>(widgetSize.y),
    //     static_cast<int>(widgetSize.x),
    //     static_cast<int>(widgetPos.y),
    //     static_cast<int>(widgetPos.x)
    // );
    
    // WidgetMaps.emplace(widget->GetUID(), new WidgetMapper(widget, win));

}
void IOController::RemoveWidget(std::string UID) {

    for (auto it = WidgetMaps.begin(); it != WidgetMaps.end(); ) {
        if (it->second->window == nullptr) { continue; } // can happen if IOController resolve is called before UIController resolve
        if (it->second->widget->GetUID() == UID) {
            // delwin(static_cast<WINDOW*>(it->second->window));
            it->second->window = nullptr;

            delete it->second;
            WidgetMaps.erase(it);
            break;
        } else { it++; }
    }

}

#pragma endregion

void IOController::Resolve() noexcept {
    // WINDOW* displayWindow = static_cast<WINDOW*>(DisplayWindow);
    // WINDOW* boxWindow = static_cast<WINDOW*>(BoxWindow);

    LOG(LogType::VITAL, "Resolving IOController");

    for (auto& [UID, map] : WidgetMaps) {
        // delwin(static_cast<WINDOW*>(map->window));
        map->window = nullptr;
        delete map;
    }

    // delwin(boxWindow);
    //
    // wrefresh(displayWindow);
    // wgetch(displayWindow);
    // delwin(displayWindow);
    // curs_set(1);
    // endwin();
}

IOController::~IOController() {
}


static constexpr char toggleCase(char c) {
    if (c >= 'a' && c <= 'z') { return c - 32; }
    if (c >= 'A' && c <= 'Z') { return c + 32; }
    return c;
}

void crashHandler(int) {
    // endwin();
    exit(1);
}

/// @returns Keycode from passed ASCII input
static constexpr Keycode GetKeycode(int ch)
{
    // ASCII
    if (ch >= 'a' && ch <= 'z') {
        return static_cast<Keycode>(static_cast<int>(Keycode::A) + (ch - 'a'));
    }
    if (ch >= 'A' && ch <= 'Z') {
        return static_cast<Keycode>(static_cast<int>(Keycode::A) + (ch - 'A'));
    }
    if (ch >= '0' && ch <= '9') {
        return static_cast<Keycode>(static_cast<int>(Keycode::Num0) + (ch - '0'));
    }

    switch (ch)
    {

        case ' ': return Keycode::Space;
        case '\n': return Keycode::Enter;
        case 27: return Keycode::Escape;
        case '\t': return Keycode::Tab;

        // case KEY_UP:    return Keycode::ArrowUp;
        // case KEY_DOWN:  return Keycode::ArrowDown;
        // case KEY_LEFT:  return Keycode::ArrowLeft;
        // case KEY_RIGHT: return Keycode::ArrowRight;
        //
        // case KEY_F(1):  return Keycode::F1;
        // case KEY_F(2):  return Keycode::F2;
        // case KEY_F(3):  return Keycode::F3;
        // case KEY_F(4):  return Keycode::F4;
        // case KEY_F(5):  return Keycode::F5;
        // case KEY_F(6):  return Keycode::F6;
        // case KEY_F(7):  return Keycode::F7;
        // case KEY_F(8):  return Keycode::F8;
        // case KEY_F(9):  return Keycode::F9;
        // case KEY_F(10): return Keycode::F10;
        // case KEY_F(11): return Keycode::F11;
        // case KEY_F(12): return Keycode::F12;

        default: return Keycode::UNKNOWN;

    }

}