#include "InputManager.hpp"

#include <SDL3/SDL_init.h>

#include "Core/GameInstance.hpp"

static constexpr Keycode GetKeycode(int ch);

InputManager::InputManager() {

    SDL_InitSubSystem(SDL_INIT_EVENTS);

    // optimize for max expected 10 keys pressed at a time
    CurrentActiveKeys.reserve(10);
}

void InputManager::BeginPlay() {

}

void InputManager::HandleInput() {
    SDL_Event event;

    // Poll all events on queue
    while (SDL_PollEvent(&event)) {

        switch (event.type) {
            case SDL_EVENT_QUIT: {
                // immediately queue resolution when window is closed
                GameInstance::Get()->RequestShutdown();
                break;
            }

            case SDL_EVENT_KEY_DOWN: {
                const Keycode k = GetKeycode(event.key.key);

                if (event.key.repeat || k == Keycode::UNKNOWN) {
                    break;
                }

                FireBinding(InputBindingsStarted, k);
                FireBinding(InputBindingsImpulse, k);

                CurrentActiveKeys.push_back(k);

                break;
            }

            case SDL_EVENT_KEY_UP: {
                const Keycode k = GetKeycode(event.key.key);

                if (event.key.repeat || k == Keycode::UNKNOWN) {
                    break;
                }

                FireBinding(InputBindingsCompleted, k);

                std::erase(CurrentActiveKeys, k);

                break;
            }

            case SDL_EVENT_MOUSE_MOTION: {

                break;
            }

            case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                LOG(LogType::INFO, "mouse button down");
                break;
            }

            default:
                LOG(LogType::WARNING, "Received unhandled event type {}", event.type);
        }


    }
}

void InputManager::_TickInput(float dt) {
    HandleInput();

    for (const Keycode k : CurrentActiveKeys) {
        FireBinding(InputBindingsOngoing, k);
    }

}

#pragma region Input Management

void InputManager::RegisterInputBinding(InputBinding binding) {

    switch (binding.type) {
        case InputType::Started:
            InputBindingsStarted[binding.key].push_back( binding );
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

void InputManager::RegisterInputBinding(std::initializer_list<InputBinding> bindings) {
    for (InputBinding binding : bindings) { RegisterInputBinding(binding); }

}

void InputManager::UnregisterInputBinding(std::string_view deleteName) {
    UnregisterBindingFrom(InputBindingsStarted, deleteName);
    UnregisterBindingFrom(InputBindingsCompleted, deleteName);
    UnregisterBindingFrom(InputBindingsOngoing, deleteName);

}

void InputManager::UnregisterAllInputBindings(void *object) {\
    UnregisterAllBindingsFrom(InputBindingsStarted, object);
    UnregisterAllBindingsFrom(InputBindingsCompleted, object);
    UnregisterAllBindingsFrom(InputBindingsOngoing, object);

}

void InputManager::UnregisterBindingFrom(BindingMap& map, std::string_view deleteName) {
    for (auto it = map.begin(); it != map.end(); ) {
        auto& vec = it->second;

        std::erase_if(vec, [deleteName](const InputBinding& binding) {
            return binding.name == deleteName;
        });

        if (vec.empty()) {
            it = map.erase(it);
        } else {
            ++it;
        }

    }

}

void InputManager::UnregisterAllBindingsFrom(BindingMap& map, void *object) {
    for (auto it = map.begin(); it != map.end(); ) {
        auto& vec = it->second;

        std::erase_if(vec, [object](const InputBinding& binding) {
            return binding.GetDelegate().GetInstance() == object;
        });

        if (vec.empty()) {
            it = map.erase(it);
        } else {
            ++it;
        }

    }

}

void InputManager::FireBinding(BindingMap& map, Keycode key) {
    const auto loc = map.find(key);
    if (loc == map.end()) { return; } // no binding yet

    for (const InputBinding& binding : loc->second ) {
        if (!binding.GetDelegate().Fire()) {
            LOG(LogType::WARNING, "Input event could not fire for binding: {}", binding.name);
        }
    }

}

#pragma endregion

void InputManager::Resolve() noexcept {
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

InputManager::~InputManager() {
}

/// @returns Keycode from passed SDL3 input
static constexpr Keycode GetKeycode(int ch)
{
    // ASCII
    if (ch >= SDLK_A && ch <= SDLK_Z) {
        return static_cast<Keycode>(static_cast<int>(Keycode::A) + (ch - SDLK_A));
    }
    if (ch >= SDLK_0 && ch <= SDLK_9) {
        return static_cast<Keycode>(static_cast<int>(Keycode::Num0) + (ch - SDLK_0));
    }

    // FUNCTION KEYS
    if (ch >= SDLK_F1 && ch <= SDLK_F12) {
        return static_cast<Keycode>(static_cast<int>(Keycode::F1) + (ch - SDLK_F1));
    }

    // ARROW KEYS
    if (ch >= SDLK_UP && ch <= SDLK_RIGHT) {
        return static_cast<Keycode>(static_cast<int>(Keycode::ArrowUp) + (ch - SDLK_UP));
    }

    switch (ch)
    {

        // SPECIAL KEYS
        case SDLK_SPACE: return Keycode::Space;
        case SDLK_RETURN: return Keycode::Enter;
        case SDLK_ESCAPE: return Keycode::Escape;
        case SDLK_TAB: return Keycode::Tab;
        case SDLK_BACKSPACE: return Keycode::Backspace;
        case SDLK_CAPSLOCK: return Keycode::CapsLock;

        // MODIFIERS
        case SDLK_LSHIFT: return Keycode::LShift;
        case SDLK_RSHIFT: return Keycode::RShift;
        case SDLK_LCTRL: return Keycode::LCtrl;
        case SDLK_RCTRL: return Keycode::RCtrl;
        case SDLK_LALT: return Keycode::LAlt;
        case SDLK_RALT: return Keycode::RAlt;

        // Other
        case SDLK_MINUS: return Keycode::Minus;
        case SDLK_EQUALS: return Keycode::Equal;
        case SDLK_SLASH: return Keycode::Slash;
        case SDLK_BACKSLASH: return Keycode::Backslash;
        case SDLK_TILDE: return Keycode::Tilde;
        case SDLK_COMMA: return Keycode::Comma;
        case SDLK_PERIOD: return Keycode::Period;
        case SDLK_SEMICOLON: return Keycode::Semicolon;
        case SDLK_APOSTROPHE: return Keycode::Apostrophe;

        case SDLK_LEFTBRACKET: return Keycode::LBracket;
        case SDLK_RIGHTBRACKET: return Keycode::RBracket;

        default: return Keycode::UNKNOWN;

    }

}