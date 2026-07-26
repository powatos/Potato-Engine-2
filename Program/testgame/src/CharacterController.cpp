
#include <Core/PotatoEngine.hpp>
#include <Core/GameInstance.hpp>
#include <Core/InputController.hpp>

#include "CharacterController.hpp"

SET_DEFAULT_SUBCLASS(PlayerController, CharacterController)

CharacterController::CharacterController() {
    CameraFollowsPlayer = true;
}

void CharacterController::SetupInputBindings() {
    PlayerController::SetupInputBindings();

    auto InputController = PotatoEngine::Get().GetInputController();

    InputController->RegisterInputBinding({
        InputBinding(Keycode::Minus, InputType::Impulse, "zoomOut", this, &CharacterController::zoomOut),
        InputBinding(Keycode::Equal, InputType::Impulse, "zoomIn", this, &CharacterController::zoomIn)
    });

}

void CharacterController::zoomOut() {
    GetCamera()->SetZoom( GetCamera()->GetZoom() - 0.5f );
}

void CharacterController::zoomIn() {
    GetCamera()->SetZoom( GetCamera()->GetZoom() + 0.5f );
}


