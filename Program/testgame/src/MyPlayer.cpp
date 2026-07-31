
#include <Core/GameInstance.hpp>

#include <Debug/Log.hpp>

#include "MyPlayer.hpp"

#include "Core/InputController.hpp"
#include "Game/Raycast.hpp"

SET_DEFAULT_SUBCLASS(Player, MyPlayer)

MyPlayer::MyPlayer() : downraycast(Raycast()) {
    SetBounce(1.f);

    SetUsingSimpleTexture(false);
    // SetTexture("Textures/smile.bmp");
}

void MyPlayer::BeginPlay() {
    downraycast.SetRay(Vector2(0,-1));
    downraycast.Settings.OverrideActorsList.insert(this);
    downraycast.Settings.OverrideActorsType = RaycastOverrideType::Exclude;
    downraycast.showDebug = false;
    downraycast.debugDuration = 1.0;

}

void MyPlayer::Tick(float dt) {
    // HitResult hit;
    // downraycast.origin = GetPosition() - Vector2::Up();
    // downraycast.Cast(hit);
    //
    // if (hit.hitActor) {
    //     LOG(LogType::DEBUG, "{}", hit.hitActor->GetSize().ToString());
    // }

    // LOG(LogType::DEBUG, "{}", GetPosition().ToStringF());
}