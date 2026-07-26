/** @file "IScreenController.cpp" */

#include "Core/IScreenController.hpp"

IScreenController::IScreenController() {
    ScreenSize = Vector2(100,100);

    FrameRate = 30.f;

    bIsResizable = true;
}

void IScreenController::SetScreenSize(Vector2 size) {
    ScreenSize = size;;
}
Vector2 IScreenController::GetScreenSize() const {
    return ScreenSize;
}


void IScreenController::SetFrameRate(float fps) {
    FrameRate = fps;
}
float IScreenController::GetFrameRate() const {
    return FrameRate;
}


void IScreenController::SetIsResizable(bool resizable) {
    bIsResizable = resizable;
}
bool IScreenController::IsResizable(bool resizable) const {
    return bIsResizable;
}

Color IScreenController::GetBackgroundColor() const {
    return BackgroundColor;
}
void IScreenController::SetBackgroundColor(Color color) {
    BackgroundColor = color;
}