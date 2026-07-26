
#include <Core/PotatoEngine.hpp>
#include <Core/GameInstance.hpp>
#include <Core/InputController.hpp>
#include <Game/Raycast.hpp>

#include "CharacterController.hpp"

// SET_DEFAULT_SUBCLASS(PlayerController, CharacterController)

CharacterController::CharacterController() {
    moveSpeed = 1.f;
    CameraFollowsPlayer = true;
}

void CharacterController::SetupInputBindings() {

    auto InputController = PotatoEngine::Get().GetInputController();

    InputController->RegisterInputBinding({
        InputBinding(Keycode::W, InputType::Impulse, "moveup", this, &CharacterController::moveup),
        InputBinding(Keycode::A, InputType::Impulse, "moveleft", this, &CharacterController::moveleft),
        InputBinding(Keycode::S, InputType::Impulse, "movedown", this, &CharacterController::movedown),
        InputBinding(Keycode::D, InputType::Impulse, "moveright", this, &CharacterController::moveright),
        InputBinding(Keycode::E, InputType::Impulse, "raycast", this, &CharacterController::rayc)
    });

}

void CharacterController::moveup() {
    GetPlayer()->AddLocalOffset(Vector2::Up() * moveSpeed);
}
void CharacterController::moveleft() {
    GetPlayer()->AddLocalOffset(-Vector2::Right() * moveSpeed);
}
void CharacterController::movedown() {
    GetPlayer()->AddLocalOffset(-Vector2::Up() * moveSpeed);
}
void CharacterController::moveright() {
    GetPlayer()->AddLocalOffset(Vector2::Right() * moveSpeed);
}

void CharacterController::rayc() {
    Raycast l{GetPlayer()->GetPosition() + Vector2(0.5f,-0.5f), -Vector2::Right()};
    Raycast d{GetPlayer()->GetPosition() + Vector2(0.5f,-0.5f), -Vector2::Up()};

    l.Settings.OverrideActorsList.insert(GetPlayer());
    l.Settings.OverrideActorsType = RaycastOverrideType::Exclude;

    d.Settings.OverrideActorsList.insert(GetPlayer());
    d.Settings.OverrideActorsType = RaycastOverrideType::Exclude;
    
    l.showDebug = true;
    d.showDebug = true;

    HitResult hitL;
    HitResult hitD;

    l.Cast(hitL);
    d.Cast(hitD);

    // if (hitL.hitActor) LOG(LogType::DEBUG, "L: {}", hitL.hitActor->ctex);
    // if (hitD.hitActor) LOG(LogType::DEBUG, "D: {}", hitD.hitActor->ctex);

}