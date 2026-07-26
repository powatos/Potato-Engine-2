/** @file Camera.cpp */

#include "Game/Camera.hpp"

Camera::Camera() {
    SetVisibility(false);

    SetSimulatingPhysics(false);
    SetCollisionType(CollisionType::None);
    SetSize(Vector2(100,100));

    SetUsingSimpleTexture(true);
    simpleColor = Color::BLANK();
}

Camera::~Camera() {
    
}
