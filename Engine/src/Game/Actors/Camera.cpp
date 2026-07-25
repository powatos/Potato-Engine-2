/** @file Camera.cpp */

#include "Game/Camera.hpp"

Camera::Camera() {
    SetVisibility(false);

    SetSimulatingPhysics(false);
    SetCollisionType(CollisionType::None);
    SetSize(Vector2(80.f, 24.f));

    SetUsingCTex(true);
    ctex = 'C';
}

Camera::~Camera() {
    
}
