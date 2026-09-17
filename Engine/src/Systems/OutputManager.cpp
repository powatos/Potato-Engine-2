#include "OutputManager.hpp"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_hints.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Core/AssetManager.hpp"
#include "Core/FontManager.hpp"
#include "Core/GameInstance.hpp"
#include "Debug/Log.hpp"
#include "UI/BoxElement.hpp"
#include "UI/ProgressBarElement.hpp"
#include "UI/TextElement.hpp"
#include "Util/GameplayHelper.hpp"

#define sdl_ren SDL_Renderer* renderer = static_cast<SDL_Renderer*>(Renderer);
#define sdl_win SDL_Window* window = static_cast<SDL_Window*>(MainWindow);

OutputManager::OutputManager() {
    LOG(LogType::VITAL, "OutputManager constructed");

    SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_ALLOW_LIBDECOR, "1");
    SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_PREFER_LIBDECOR, "1");

    SDL_InitSubSystem(SDL_INIT_VIDEO);

    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_CreateWindowAndRenderer(
        Internal::EngineGlobals::appName.c_str(),
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

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    TTF_Init();

    TTF_TextEngine* textEngine = TTF_CreateRendererTextEngine(renderer);
    TextEngine = textEngine;

    bootstrapScreenEnabled = false;
}



void OutputManager::BeginPlay() {
    GameInstance* instance = GameInstance::Get();

}

void OutputManager::DrawLevel() {
    sdl_ren

    SDL_SetRenderDrawColor(renderer, backgroundColor.R, backgroundColor.G, backgroundColor.B, backgroundColor.A);

    { // toggle comment this whole block for bars
        SDL_RenderClear(renderer);
        // SDL_FRect sky {0,0,screenResolution.x, screenResolution.y};
        // SDL_RenderFillRect(renderer, &sky);
    }

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

            SDL_Texture* tex = static_cast<SDL_Texture*>(actor_tex->Internal_GetTexture());
            // SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_PIXELART);
            SDL_RenderTexture(renderer, tex, nullptr, &rect);

        }

    }

}

#pragma region UI Rendering

void OutputManager::DrawHUD() {

    const UIHierarchy* root = UIManager::Get()->RootWidget_H;

    const Vector2 pos = Vector2();;
    const Vector2 size = screenResolution;

    Recurse_DrawUI(root, pos, size);

}

void OutputManager::Recurse_DrawUI(const UIHierarchy* ui, Vector2 parentAbsPos, Vector2 parentAbsSize) {

    UIElement* uiObj = ui->Object;

    if (!uiObj->isVisible()) { return; }

    const UIVector localPos = uiObj->GetScreenPosition();
    const UIVector localSize = uiObj->GetScreenSize();

    const Vector2 absPos = parentAbsPos + parentAbsSize * localPos.scale + localPos.offset;
    const Vector2 absSize = parentAbsSize * localSize.scale + localSize.offset;

    // calls corresponding Render() overload
    uiObj->Internal_RenderPassthrough(this, absPos, absSize);

    for (const UIHierarchy* child : ui->Children) {
        Recurse_DrawUI(child, absPos, absSize);
    }

}

void OutputManager::Render(const class Widget* ui, Vector2 uiPos, Vector2 uiSize) {
}

void OutputManager::Render(const class TextElement* ui, Vector2 uiPos, Vector2 uiSize) {
    sdl_ren

    TTF_Text* text = static_cast<TTF_Text*>(ui->Internal_GetText());

    TTF_DrawRendererText(text, uiPos.x, uiPos.y);

}

void OutputManager::Render(const class BoxElement* ui, Vector2 uiPos, Vector2 uiSize) {
    sdl_ren

    const SDL_FRect rect{
        uiPos.x,
        uiPos.y,
        uiSize.x,
        uiSize.y
    };

    if (ui->BorderWeight != 0) {
        const int t = ui->BorderWeight;

        const SDL_FRect rects[4] = {
            { // top
                rect.x - t,
                rect.y - t,
                rect.w + t*2,
                static_cast<const float>(t)
            },
            { // bottom
                rect.x - t,
                rect.y + rect.h,
                rect.w + t*2,
                static_cast<const float>(t)
            },
            { // left
                rect.x - t,
                rect.y,
                static_cast<const float>(t),
                rect.h
            },
            { // right
                rect.x + rect.w,
                rect.y,
                static_cast<const float>(t),
                rect.h
            }
        };

        const Color col = ui->BorderColor;
        SDL_SetRenderDrawColor(renderer, col.R, col.G, col.B, col.A);

        SDL_RenderFillRects(renderer, rects, 4);
    }

    const Color col = ui->FillColor;
    SDL_SetRenderDrawColor(renderer, col.R, col.G, col.B, col.A);

    SDL_RenderFillRect(renderer, &rect);

}

void OutputManager::Render(const ProgressBarElement* ui, Vector2 uiPos, Vector2 uiSize) {
    sdl_ren

    if (ui->maxValue == 0.f) {
        LOG(LogType::WARNING, "Property bar with 0 maxValue not rendered");
        return;
    }

    SDL_FRect outer {
        uiPos.x,
        uiPos.y,
        uiSize.x,
        uiSize.y
    };
    SDL_FRect bar {
        uiPos.x,
        uiPos.y,
        (ui->value / ui->maxValue) * uiSize.x,
        uiSize.y
    };

    const Color bgC = ui->backgroundColor;
    const Color bC = ui->barColor;

    SDL_SetRenderDrawColor(renderer, bgC.R, bgC.G, bgC.B, bgC.A);
    SDL_RenderFillRect(renderer, &outer);

    SDL_SetRenderDrawColor(renderer, bC.R, bC.G, bC.B, bC.A);
    SDL_RenderFillRect(renderer, &bar);
}

#pragma endregion

void OutputManager::DrawBars() {
    sdl_ren

    SDL_SetRenderLogicalPresentation(renderer, static_cast<int>(screenResolution.x), static_cast<int>(screenResolution.y), SDL_LOGICAL_PRESENTATION_DISABLED);

    SDL_SetRenderDrawColor(renderer, 0,0,0,0xff);
    SDL_RenderClear(renderer);

    const auto mode = static_cast<SDL_RendererLogicalPresentation>(SDL_LOGICAL_PRESENTATION_DISABLED + static_cast<int>(rescaleMode));
    SDL_SetRenderLogicalPresentation(renderer, static_cast<int>(screenResolution.x), static_cast<int>(screenResolution.y), mode);

}


void OutputManager::Draw() {
    sdl_ren

    // DrawBars();

    DrawLevel();

    DrawHUD();

    SDL_RenderPresent(renderer);

}

void OutputManager::_TickRender(float dt) {
    Draw();
}

void OutputManager::Tick(float dt) {
    // TODO: update windowSize member when window size changed event is triggered instead of in tick
    // sdl_win

    // int w, h;
    // SDL_GetWindowSize(window, &w, &h);
    //
    // windowSize = Vector2(static_cast<float>(w),static_cast<float>(h));
}

void* OutputManager::RequestRenderingContext() const {
    return Renderer;
}

void* OutputManager::RequestTTFEngine() const {
    return TextEngine;
}

void OutputManager::ShowBootstrapScreen() {
    sdl_ren

    if (renderer == nullptr) { return; }

    const std::string text = "Loading Potato Engine...";
    const Vector2 pos = screenResolution/2;

    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderDebugText(renderer, pos.x, pos.y, text.c_str());
    SDL_RenderPresent(renderer);

    bootstrapScreenEnabled = true;
}
void OutputManager::HideBootstrapScreen() {
    sdl_ren

    if (renderer == nullptr) { return; }
    if (!bootstrapScreenEnabled) { return; }

    SDL_RenderClear(renderer);

}

#pragma region Settings Setters

void OutputManager::SetScreenResolution(const Vector2& resolution) {
    sdl_ren

    screenResolution = resolution;

    SetRescaleMode(rescaleMode);
}

void OutputManager::SetWindowSize(const Vector2& size) {
    sdl_win

    SDL_SetWindowSize(window, static_cast<int>(size.x), static_cast<int>(size.y));
    windowSize = size;

}

void OutputManager::SetWindowMode(WindowMode mode) {
    sdl_win

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
    sdl_ren

    const auto _mode = static_cast<SDL_RendererLogicalPresentation>(SDL_LOGICAL_PRESENTATION_DISABLED + static_cast<int>(mode));
    SDL_SetRenderLogicalPresentation(renderer, static_cast<int>(screenResolution.x), static_cast<int>(screenResolution.y), _mode);

    rescaleMode = mode;
}

void OutputManager::SetShowBorder(bool show) {
    sdl_win

    SDL_SetWindowBordered(window, false);
}

void OutputManager::SetIsResizable(bool isResizable) {
    sdl_win

    SDL_SetWindowResizable(window, isResizable);
}

#pragma endregion


void OutputManager::Resolve() noexcept {
    LOG(LogType::VITAL, "Resolving OutputManager");

    TTF_Quit();

    SDL_DestroyRenderer(static_cast<SDL_Renderer*>(Renderer));
    SDL_DestroyWindow(static_cast<SDL_Window*>(MainWindow));

    SDL_QuitSubSystem(SDL_INIT_VIDEO);

}

OutputManager::~OutputManager() {
}
