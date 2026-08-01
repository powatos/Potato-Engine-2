#include "OutputManager.hpp"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_hints.h>

#include "Core/GameInstance.hpp"
#include "UI/TextElement.hpp"
#include "UI/Widget.hpp"
#include "Util/GameplayHelper.hpp"


OutputManager::OutputManager() {
    LOG(LogType::VITAL, "OutputManager constructed");

    SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_ALLOW_LIBDECOR, "1");
    SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_PREFER_LIBDECOR, "1");

    SDL_InitSubSystem(SDL_INIT_VIDEO);

    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_CreateWindowAndRenderer(
        ___ENGINE_GLOBALS::appName.c_str(),
        static_cast<int>(windowSize.x), static_cast<int>(windowSize.y),
        SDL_WINDOW_MOUSE_FOCUS |
        SDL_WINDOW_INPUT_FOCUS |
        (windowMode == WindowMode::Windowed ? SDL_WINDOW_RESIZABLE : 0x0) |
        (windowMode == WindowMode::Fullscreen ? SDL_WINDOW_FULLSCREEN : 0x0) |
        (ShowBorder ? SDL_WINDOW_BORDERLESS : 0x0),
        &window,
        &renderer
    );
    MainWindow = window;
    Renderer = renderer;

    if (MainWindow == nullptr || Renderer == nullptr) {
        LOG(LogType::ERROR, "Failed to create display context: {}", SDL_GetError());
    }

    SetScreenResolution(screenResolution);

    SDL_SetWindowResizable(window, IsResizable);
}



void OutputManager::BeginPlay() {
    GameInstance* instance = GameInstance::Get();

}

void OutputManager::DrawLevel() {
    SDL_Renderer* renderer = static_cast<SDL_Renderer*>(Renderer);

    SDL_SetRenderDrawColor(renderer, backgroundColor.R, backgroundColor.G, backgroundColor.B, backgroundColor.A);

    // SDL_RenderClear(renderer);
    SDL_FRect sky {0,0,screenResolution.x, screenResolution.y};
    SDL_RenderFillRect(renderer, &sky);

    const GameInstance* Instance = GameInstance::Get();
    const World* world = Instance->GetWorld();
    const ActorPool& renderActors = world->GetAllActors();

    const Camera* camera = Instance->GetPlayerController()->GetCamera();

    const float aspectRatio = screenResolution.x / screenResolution.y;
    const float cameraRenderHeight = camera->GetViewHeight() / camera->GetZoom();
    
    for ( Actor* actor : renderActors ) {
        if (actor == nullptr) {
            LOG(LogType::WARNING, "nullptr actor found in actor pool while drawing - skipped actor");
            continue;
        }

        if (!actor->isVisible()) { continue; }

        const Vector2 actorPos = actor->GetPosition();
        const Vector2 actorSize = actor->GetSize();
        // const float rot = actor->GetTexture().GetRotation();

        if (!GameplayHelper::IsActorOverlapping(actor, camera)) { continue; }

        // position offset relative to camera AND screen AND scaled to resolution
        const Vector2 ScaleFactor = Vector2( screenResolution / camera->GetSize() );
        const Vector2 screenVector =  Vector2(
            (actorPos.x - camera->GetPosition().x),
            (camera->GetPosition().y - actorPos.y)
        ) * ScaleFactor;

        // const float sine = std::sin(rot * PI/180.0);
        // const float cosine = std::cos(rot * PI/180.0);
        //
        // const Vector2 rotVec {
        //     screenVector.x * cosine - screenVector.y * sine,
        //     screenVector.x * sine + screenVector.y * cosine
        // };

        SDL_FRect rect {
            screenVector.x,
            screenVector.y,
            actorSize.x * ScaleFactor.x,
            actorSize.y * ScaleFactor.y
        };

        if (actor->IsUsingSimpleTexture()) {


            const Color col = actor->simpleColor;

            SDL_SetRenderDrawColor(renderer, col.R, col.G, col.B, col.A);

            if (actor->simpleColorFill) {
                SDL_RenderFillRect(
                    renderer,
                    &rect
                );
            } else {
                SDL_RenderRect(
                    renderer,
                    &rect
                );
            }
        } else {
            Texture* actor_tex = actor->GetTexture();
            if (actor_tex == nullptr) {
                continue;
            }

            SDL_Texture* tex = static_cast<SDL_Texture*>(actor_tex->___Get_Texture());
            // SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_PIXELART);
            SDL_RenderTexture(renderer, tex, nullptr, &rect);

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

void OutputManager::DrawBars() {
    SDL_Renderer* renderer = static_cast<SDL_Renderer*>(Renderer);

    SDL_SetRenderLogicalPresentation(renderer, static_cast<int>(screenResolution.x), static_cast<int>(screenResolution.y), SDL_LOGICAL_PRESENTATION_DISABLED);

    SDL_SetRenderDrawColor(renderer, 0,0,0,0xff);
    SDL_RenderClear(renderer);

    const auto mode = static_cast<SDL_RendererLogicalPresentation>(SDL_LOGICAL_PRESENTATION_DISABLED + static_cast<int>(rescaleMode));
    SDL_SetRenderLogicalPresentation(renderer, static_cast<int>(screenResolution.x), static_cast<int>(screenResolution.y), mode);

}


void OutputManager::Draw() {
    SDL_Renderer* renderer = static_cast<SDL_Renderer*>(Renderer);

    DrawBars();

    DrawLevel();

    // DrawHUD();

    SDL_RenderPresent(renderer);
}

void OutputManager::_TickRender(float dt) {
    Draw();
}

void OutputManager::Tick(float dt) {
    // TODO: update windowSize member when window size changed event is triggered instead of in tick
    // SDL_Window* window = static_cast<SDL_Window*>(MainWindow);

    // int w, h;
    // SDL_GetWindowSize(window, &w, &h);
    //
    // windowSize = Vector2(static_cast<float>(w),static_cast<float>(h));
}

void* OutputManager::RequestRenderingContext() const {
    return Renderer;
}

#pragma region Settings Setters

void OutputManager::SetScreenResolution(const Vector2& resolution) {
    SDL_Renderer* renderer = static_cast<SDL_Renderer*>(Renderer);

    screenResolution = resolution;

    SetRescaleMode(rescaleMode);
}

void OutputManager::SetWindowSize(const Vector2& size) {
    SDL_Window* window = static_cast<SDL_Window*>(MainWindow);

    SDL_SetWindowSize(window, static_cast<int>(size.x), static_cast<int>(size.y));
    windowSize = size;

}

void OutputManager::SetWindowMode(WindowMode mode) {
    SDL_Window* window = static_cast<SDL_Window*>(MainWindow);

    switch (mode) {
        case WindowMode::Fullscreen: {
            SetIsResizable(false);
            SDL_SetWindowFullscreen(window, true);
            break;
        }
        case WindowMode::Windowed: {
            SDL_SetWindowFullscreen(window, false);
            SDL_SetWindowBordered(window, true);
            SetIsResizable(true);

            break;
        }
    }

    windowMode = mode;
}

void OutputManager::SetRescaleMode(WindowRescaleMode mode) {
    SDL_Renderer* renderer = static_cast<SDL_Renderer*>(Renderer);

    const auto _mode = static_cast<SDL_RendererLogicalPresentation>(SDL_LOGICAL_PRESENTATION_DISABLED + static_cast<int>(mode));
    SDL_SetRenderLogicalPresentation(renderer, static_cast<int>(screenResolution.x), static_cast<int>(screenResolution.y), _mode);

    rescaleMode = mode;
}

void OutputManager::SetShowBorder(bool show) {
    SDL_Window* window = static_cast<SDL_Window*>(MainWindow);

    SDL_SetWindowBordered(window, false);
}

void OutputManager::SetIsResizable(bool isResizable) {
    SDL_Window* window = static_cast<SDL_Window*>(MainWindow);

    SDL_SetWindowResizable(window, isResizable);
}

#pragma endregion

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


#pragma endregion

void OutputManager::Resolve() noexcept {
    LOG(LogType::VITAL, "Resolving OutputManager");

    for (auto& [UID, map] : WidgetMaps) {
        // delwin(static_cast<WINDOW*>(map->window));
        map->window = nullptr;
        delete map;
    }

    SDL_DestroyRenderer(static_cast<SDL_Renderer*>(Renderer));
    SDL_DestroyWindow(static_cast<SDL_Window*>(MainWindow));

    SDL_QuitSubSystem(SDL_INIT_VIDEO);

}

OutputManager::~OutputManager() {
}

