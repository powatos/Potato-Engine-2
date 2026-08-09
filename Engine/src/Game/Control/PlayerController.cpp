/** @file PlayerController.cpp */

#include <algorithm>

#include "Core/GameInstance.hpp"
#include "Core/EventManager.hpp"
#include "Core/InputController.hpp"
#include "Core/PotatoEngine.hpp"
#include "Actors/Camera.hpp"
#include "Actors/Player.hpp"
#include "Game/World.hpp"

#include "Debug/Log.hpp"
#include "Util/GameplayHelper.hpp"

#include "Game/PlayerController.hpp"

PlayerController::PlayerController() {
    TargetMovementVelocity = 1000.f;
    JumpForce = 20.f;

    CameraFollowsPlayer = true;
}

void PlayerController::Initialize() {

    World* world = GameInstance::Get()->GetWorld();

    // Create camera
    ActiveCamera = world->SpawnActor<Camera>();

}

void PlayerController::Tick(float dt) {
    Tickable::Tick(dt);
    
    if (CameraFollowsPlayer) { /// Camera follow logic
        if (ActivePlayer == nullptr) { return; }
        World* world = GameInstance::Get()->GetWorld();
        const Vector2 playerPos = ActivePlayer->GetPosition();
        const Vector2 cameraSize = ActiveCamera->GetSize();

        Vector2 newCamPos;

        newCamPos.x = playerPos.x - cameraSize.x / 2;
        newCamPos.y = playerPos.y + cameraSize.y / 2;

        // newCamPos.x = std::clamp(newCamPos.x, 0.f, world->Settings.Size.x - ActiveCamera->GetSize().x);
        // newCamPos.y = std::clamp(newCamPos.y, ActiveCamera->GetSize().y, world->Settings.Size.y);

        ActiveCamera->SetPosition(newCamPos + CameraOffset);
    }
    
    ActivePlayer->AddForce(playerMoveVec * TargetMovementVelocity);
}

void PlayerController::AssignPlayer(Player* player) {
    ActivePlayer = player;
    if (ActivePlayer == nullptr) {
        LOG(LogType::WARNING, "Player set to nullptr during controller assignment");
    }
}

Player* PlayerController::GetPlayer() const { return ActivePlayer; }
Camera* PlayerController::GetCamera() const { return ActiveCamera; }

void PlayerController::SetupInputBindings() {

    // setup input bindings
    IInputController* controller = PotatoEngine::Get().GetInputController();

    controller->RegisterInputBinding({
        InputBinding(Keycode::A, InputType::Started, "StartMoveLeft", this, &PlayerController::sMvL),
        InputBinding(Keycode::D, InputType::Started, "StartMoveRight", this, &PlayerController::sMvR),
        
        InputBinding(Keycode::Space, InputType::Impulse, "Jump", this, &PlayerController::jump),
        
        InputBinding(Keycode::A, InputType::Completed, "CompleteMoveLeft", this, &PlayerController::eMvL),
        InputBinding(Keycode::D, InputType::Completed, "CompleteMoveRight", this, &PlayerController::eMvR),

        InputBinding(Keycode::G, InputType::Impulse, "_rotate", this, &PlayerController::_rot)
    });
}

#pragma region Move functions

void PlayerController::sMvL() { playerMoveVec.x = -1; }
void PlayerController::sMvR() { playerMoveVec.x =  1; }

void PlayerController::jump() { ActivePlayer->AddImpulse(Vector2(0, JumpForce)); }

void PlayerController::eMvL() { playerMoveVec.x = 0; }
void PlayerController::eMvR() { playerMoveVec.x = 0; }

void PlayerController::_rot() {
    // ActivePlayer->GetTexture().AddLocalRotation(15);
}

#pragma endregion

PlayerController::~PlayerController() {
    PotatoEngine::Get().GetInputController()->UnregisterAllInputBindings(this);
}