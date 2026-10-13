
#include <Core/GameInstance.hpp>

#include <Debug/Log.hpp>

#include "PlatformPlayer.hpp"

#include "Core/AssetManager.hpp"
#include "Core/InputController.hpp"
#include "Debug/DebugObjectManager.hpp"
#include "Game/Raycast.hpp"

SET_DEFAULT_SUBCLASS(Player, PlatformPlayer)

PlatformPlayer::PlatformPlayer() {
    SetBounce(1.f);

    SetUsingSimpleTexture(false);
    Texture* tex = AssetManager::Get()->GetAsset<Texture>("Capsule.bmp");
    tex->SetKeyColor(Color(0xff'ff'00'ff));
    SetTexture(tex);
    ResizeToTexture();

    SetUseAsymmetricGravity(true);

    SetSize(Vector2(5, 10));
    SetZIndex(5);
}

void PlatformPlayer::BeginPlay() {
}

void PlatformPlayer::Tick(float dt) {

}