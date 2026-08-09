/** @file "IWindowController.cpp" */

#include "Core/IWindowController.hpp"

IWindowController::IWindowController()
    : screenResolution(Vector2(1280, 720)),
      windowSize(Vector2(1280, 720)),
      targetFrameRate(30.f),
      backgroundColor(Color()),
      windowMode(WindowMode::Windowed),
      rescaleMode(WindowRescaleMode::Letterbox),
      ShowBorder(true),
      IsResizable(true)
{}

Vector2 IWindowController::GetScreenResolution() const {
    return screenResolution;
}

Vector2 IWindowController::GetWindowSize() const {
    return windowSize;
}

float IWindowController::GetTargetFrameRate() const {
    return targetFrameRate;
}

Color IWindowController::GetBackgroundColor() const {
    return backgroundColor;
}

WindowMode IWindowController::GetWindowMode() const {
    return windowMode;
}

WindowRescaleMode IWindowController::GetRescaleMode() const {
    return rescaleMode;
}

bool IWindowController::GetShowBorder() const {
    return ShowBorder;
}

bool IWindowController::GetIsResizable() const {
    return IsResizable;
}

void IWindowController::SetTargetFrameRate(float frameRate) {
    targetFrameRate = frameRate;
}

void IWindowController::SetBackgroundColor(const Color& color) {
    backgroundColor = color;
}
