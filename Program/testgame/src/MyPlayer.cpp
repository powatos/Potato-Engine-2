
#include <Core/GameInstance.hpp>

#include <Debug/Log.hpp>

#include "MyPlayer.hpp"

#include "Core/AssetManager.hpp"
#include "Core/InputController.hpp"
#include "Debug/DebugObjectManager.hpp"
#include "Game/Raycast.hpp"

SET_DEFAULT_SUBCLASS(Player, MyPlayer)

MyPlayer::MyPlayer() : downraycast(Raycast()) {
    SetBounce(1.f);

    SetUsingSimpleTexture(false);

    Texture* tex = AssetManager::Get()->GetAsset<Texture>("Capsule.bmp");
    tex->SetKeyColor(Color(0xff'ff'00'ff));
    SetTexture(tex);

    SetSize(Vector2(5, 10));

    SetZIndex(5);
}

void MyPlayer::BeginPlay() {
    downraycast.SetRay(Vector2(0,10));
    downraycast.Settings.OverrideActorsList.insert(this);
    downraycast.Settings.OverrideActorsType = RaycastOverrideType::Exclude;
    downraycast.showDebug = false;
    downraycast.debugDuration = 1.0;

}

void MyPlayer::Tick(float dt) {

    DebugObjects::Circle(
        GetPosition(),
        10,
        1
    );

    // HitResult hit;
    // downraycast.origin = GetPosition() - Vector2::Up();
    // downraycast.Cast(hit);
    //
    // if (hit.hitActor) {
    //     LOG(LogType::DEBUG, "{}", hit.hitActor->GetSize().ToString());
    // }

    // LOG(LogType::DEBUG, "{}", GetPosition().ToStringF());
}