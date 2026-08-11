
#include <Core/GameInstance.hpp>

#include <Debug/Log.hpp>

#include "MyPlayer.hpp"

#include "Core/InputController.hpp"
#include "Core/TextureManager.hpp"
#include "Game/Raycast.hpp"

SET_DEFAULT_SUBCLASS(Player, MyPlayer)

MyPlayer::MyPlayer() : downraycast(Raycast()) {
    SetBounce(1.f);

    SetUsingSimpleTexture(false);

    Texture* tex = TextureManager::Get()->GetTexture("Capsule.bmp");
    tex->SetKeyColor(Color(0xff'ff'00'ff));
    SetTexture(tex);

    SetSize(Vector2(5, 10));
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