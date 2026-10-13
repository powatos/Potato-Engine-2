
#include <Core/PotatoEngine.hpp>
#include <Core/GameInstance.hpp>
#include <Core/InputController.hpp>

#include "CharacterController.hpp"

SET_DEFAULT_SUBCLASS(PlayerController, CharacterController)

CharacterController::CharacterController() {
    CameraFollowsPlayer = false;

    JumpForce = 63.f;
}

void CharacterController::SetupInputBindings() {
    PlayerController::SetupInputBindings();

    auto InputController = PotatoEngine::GetInputController();
    
}

void CharacterController::BeginPlay() {
    PlayerController::BeginPlay();

    GetCamera()->SetPosition(Vector2(-50,105));
    GetCamera()->SetViewHeight(110);

}

