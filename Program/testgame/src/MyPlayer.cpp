
#include <Core/GameInstance.hpp>

#include <Debug/Log.hpp>

#include "MyPlayer.hpp"

#include "Game/Raycast.hpp"

SET_DEFAULT_SUBCLASS(Player, MyPlayer)

MyPlayer::MyPlayer() : downraycast(Raycast()) {
    SetBounce(1.f);
}

void MyPlayer::BeginPlay() {
    downraycast.SetRay(Vector2(5,-5));
    downraycast.Settings.OverrideActorsList.insert(this);
    downraycast.Settings.OverrideActorsType = RaycastOverrideType::Exclude;
    downraycast.showDebug = false;
    downraycast.debugDuration = 1.0;
}

void MyPlayer::Tick(float dt) {
    // HitResult hit;
    // downraycast.origin = GetPosition() + Vector2::Right() * GetSize().x;
    // downraycast.Cast(hit);

    // // if (hit.hitActor) {
    // //     LOG(LogType::DEBUG, "{}", hit.distance);
    // // }

    LOG(LogType::DEBUG, "{}", GetPosition().ToStringF());
}