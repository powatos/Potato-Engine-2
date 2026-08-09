/** @file Camera.cpp */

#include "Actors/Camera.hpp"

#include "Core/IWindowController.hpp"
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

    IWindowController* windowController = PotatoEngine::Get().GetWindowController();

    const float aspectRatio = windowController->GetScreenResolution().x / windowController->GetScreenResolution().y;
    const float renderHeight = ViewHeight / Zoom;
    SetSize(Vector2(renderHeight * aspectRatio, renderHeight));

}
