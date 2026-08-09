/** @file UIElement.cpp */

#include "UI/UIElement.hpp"

#include "Core/PotatoEngine.hpp"

UIElement::UIElement(const std::string& UID) : UID(UID), Visible(true) {}

void UIElement::Setup() {}

const std::string& UIElement::GetUID() const {
    return UID;
}

UIVector UIElement::GetScreenSize() const {
    return ScreenSize;
}
void UIElement::SetScreenSize(const UIVector& size) {
    ScreenSize = size;
}


UIVector UIElement::GetScreenPosition() const {
    return ScreenPosition; 
}
void UIElement::SetScreenPosition(const UIVector& ScreenPos) {
    ScreenPosition = ScreenPos;
}

void UIElement::AddScreenOffset(const UIVector& ScreenOffset) {
    SetScreenPosition(ScreenPosition + ScreenOffset);
}


bool UIElement::isVisible() const {
    return Visible;
}
void UIElement::SetVisibility(bool visibility) {
    Visible = visibility;
}
void UIElement::ToggleVisibility() {
    Visible = !Visible;
}

Vector2 UIElement::GetTruePosition() const {
    return GetScreenPosition().scale * PotatoEngine::Get().GetWindowController()->GetScreenResolution() + GetScreenPosition().offset;

}
Vector2 UIElement::GetTrueSize() const {
    return GetScreenSize().scale * PotatoEngine::Get().GetWindowController()->GetScreenResolution() + GetScreenSize().offset;

}
