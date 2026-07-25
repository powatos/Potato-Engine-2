/** @file UIElement.cpp */

#include "UI/UIElement.hpp"

Vector2 UIElement::GetScreenSize() const {
    return ScreenSize;
}
void UIElement::SetScreenSize(const Vector2& size) {
    ScreenSize = size;
}


Vector2 UIElement::GetScreenPosition() const { 
    return ScreenPosition; 
}
void UIElement::SetScreenPosition(const Vector2 &ScreenPos) { 
    ScreenPosition = ScreenPos;
}

void UIElement::AddScreenOffset(const Vector2& ScreenOffset) {
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
