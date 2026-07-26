#include "OutputManager.hpp"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>

#include "Core/GameInstance.hpp"
#include "UI/TextElement.hpp"
#include "UI/Widget.hpp"
#include "Util/GameplayHelper.hpp"


OutputManager::OutputManager() {
    LOG(LogType::VITAL, "OutputManager constructed");

    SDL_InitSubSystem(SDL_INIT_VIDEO);

    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_CreateWindowAndRenderer(
        ___ENGINE_GLOBALS::appName.c_str(),
        ScreenSize.x, ScreenSize.y,
        SDL_WINDOW_RESIZABLE |
        SDL_WINDOW_MOUSE_FOCUS |
        SDL_WINDOW_INPUT_FOCUS,
        &window,
        &renderer
    );

    MainWindow = window;
    Renderer = renderer;

}



void OutputManager::BeginPlay() {
    GameInstance* instance = GameInstance::Get();

}

void OutputManager::DrawLevel() {
    SDL_Renderer* renderer = static_cast<SDL_Renderer*>(Renderer);

    SDL_SetRenderDrawColor(renderer, BackgroundColor.R, BackgroundColor.G, BackgroundColor.B, BackgroundColor.A);

    SDL_RenderClear(renderer);

    const GameInstance* Instance = GameInstance::Get();
    const World* world = Instance->GetWorld();
    const ActorPool& renderActors = world->GetAllActors();

    const Camera* camera = Instance->GetPlayerController()->GetCamera();

    for ( Actor* actor : renderActors ) {
        if (actor == nullptr) {
            LOG(LogType::WARNING, "nullptr actor found in actor pool while drawing - skipped actor");
            continue;
        }

        if (!actor->isVisible()) { continue; }

        const Vector2 actorPos = actor->GetPosition();
        const Vector2 actorSize = actor->GetSize();
        const float rot = actor->GetTexture().GetRotation();

        if (!GameplayHelper::IsActorOverlapping(actor, camera)) { continue; }

        // offseted relative to camera AND screen
        const Vector2 screenVector = Vector2(
            (actorPos.x - camera->GetPosition().x) * (ScreenSize.x / camera->GetSize().x),
            (camera->GetPosition().y - actorPos.y) * (ScreenSize.y / camera->GetSize().y)
        );

        // const float sine = std::sin(rot * PI/180.0);
        // const float cosine = std::cos(rot * PI/180.0);
        //
        // const Vector2 rotVec {
        //     screenVector.x * cosine - screenVector.y * sine,
        //     screenVector.x * sine + screenVector.y * cosine
        // };

        if (actor->IsUsingSimpleTexture()) {
            SDL_FRect rect {
                screenVector.x,
                screenVector.y,
                actorSize.x,
                actorSize.y
            };

            const Color col = actor->simpleColor;

            SDL_SetRenderDrawColor(renderer, col.R, col.G, col.B, col.A);

            SDL_RenderRect(
                renderer,
                &rect
            );
        }

    }


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
    SDL_Renderer* renderer = static_cast<SDL_Renderer*>(Renderer);

    DrawLevel();
    // DrawHUD();

    SDL_RenderPresent(renderer);
}

void OutputManager::_TickRender(float dt) {
    Draw();
}

void OutputManager::Tick(float dt) {
    SDL_Window* window = static_cast<SDL_Window*>(MainWindow);

    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    ScreenSize = Vector2(w,h);
}

#pragma region Widget Management

void OutputManager::RegisterWidget(Widget *widget) {
    if (MainWindow == nullptr) { return; }
    // WINDOW* displayWindow = static_cast<WINDOW*>(MainWindow);

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

void OutputManager::SetScreenSize(Vector2 size) {
    IScreenController::SetScreenSize(size);

    SDL_Window* window = static_cast<SDL_Window*>(MainWindow);

    SDL_SetWindowSize(window, size.x, size.y);
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

