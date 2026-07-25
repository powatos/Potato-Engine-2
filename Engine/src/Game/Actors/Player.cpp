/** @file Player.cpp */

#include "Core/GameInstance.hpp"
#include "Core/InputController.hpp"
#include "Core/PotatoEngine.hpp"

#include "Debug/Log.hpp"
#include "Util/GameplayHelper.hpp"

#include "Game/Player.hpp"

Player::Player() {
    SetSize(Vector2(2,2));
    SetBounce(0.6f);
    SetUseAsymmetricGravity(true);
    SetSimulatingPhysics(true);

    SetTexture("Player_default");
    ResizeToTexture();

}

void Player::BeginPlay() {
    Actor::BeginPlay();

}

void Player::Tick([[maybe_unused]] float dt) {
    Entity::Tick(dt);

}

Player::~Player() {
}

