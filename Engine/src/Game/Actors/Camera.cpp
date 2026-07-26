/** @file Camera.cpp */

#include "Game/Camera.hpp"

#include "Core/IScreenController.hpp"
#include "Core/PotatoEngine.hpp"

Camera::Camera() {
    SetVisibility(false);

    SetSimulatingPhysics(false);
    SetCollisionType(CollisionType::None);
    // SetSize(Vector2(100,100));

    SetUsingSimpleTexture(true);
    simpleColor = Color::BLANK();

    Zoom = 1.f;
    ViewHeight = 100;
}

Camera::~Camera() {

}

void Camera::SetZoom(float zoom) {
    Zoom = zoom;
}

float Camera::GetZoom() const {
    return Zoom;
}

void Camera::SetViewHeight(float height) {
    ViewHeight = height;
}

float Camera::GetViewHeight() const {
    return ViewHeight;
}

void Camera::TickPostUpdate(float dt) {
    Actor::TickPostUpdate(dt);

    IScreenController* screenController = PotatoEngine::Get().GetScreenController();

    const float aspectRatio = screenController->GetScreenResolution().x / screenController->GetScreenResolution().y;
    const float renderHeight = ViewHeight / Zoom;
    SetSize(Vector2(renderHeight * aspectRatio, renderHeight));

}
