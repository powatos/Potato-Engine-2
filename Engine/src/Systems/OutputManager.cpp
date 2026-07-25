#include "OutputManager.hpp"

#include <SDL3/SDL_init.h>

#include "Core/GameInstance.hpp"
#include "UI/TextElement.hpp"
#include "UI/Widget.hpp"
#include "Util/GameplayHelper.hpp"


OutputManager::OutputManager() : FRAMES_PER_SECOND(30.f) {
    LOG(LogType::VITAL, "OutputManager constructed");

    SDL_InitSubSystem(SDL_INIT_VIDEO);

    DisplayWindow = SDL_CreateWindow(___ENGINE_GLOBALS::appName.c_str(), 500,500,
        SDL_WINDOW_RESIZABLE |
        SDL_WINDOW_MOUSE_GRABBED |
        SDL_WINDOW_MOUSE_FOCUS |
        SDL_WINDOW_INPUT_FOCUS

    );

}



void OutputManager::BeginPlay() {
    GameInstance* instance = GameInstance::Get();

    FRAMES_PER_SECOND = instance->FRAMES_PER_SECOND;


}

void OutputManager::DrawLevel() {
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

void OutputManager::DrawHUD() {
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

void OutputManager::Draw() {
    DrawLevel();
    DrawHUD();

    // doupdate();
}

void OutputManager::_TickRender(float dt) {
    Draw();
}

#pragma region Widget Management

void OutputManager::RegisterWidget(Widget *widget) {
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

void OutputManager::RemoveWidget(std::string UID) {

    for (auto it = WidgetMaps.begin(); it != WidgetMaps.end(); ) {
        if (it->second->window == nullptr) { continue; } // can happen if OutputManager resolve is called before UIController resolve
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


void OutputManager::Resolve() noexcept {
    LOG(LogType::VITAL, "Resolving OutputManager");

    for (auto& [UID, map] : WidgetMaps) {
        // delwin(static_cast<WINDOW*>(map->window));
        map->window = nullptr;
        delete map;
    }

    SDL_QuitSubSystem(SDL_INIT_VIDEO);

}

OutputManager::~OutputManager() {
}

