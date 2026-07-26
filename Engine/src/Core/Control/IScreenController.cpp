/** @file "IScreenController.cpp" */

#include "Core/IScreenController.hpp"

IScreenController::IScreenController()
    : screenResolution(Vector2(1280, 720)),
      windowSize(Vector2(1280, 720)),
      targetFrameRate(30.f),
      backgroundColor(Color()),
      windowMode(WindowMode::Windowed),
      rescaleMode(WindowRescaleMode::Letterbox),
      ShowBorder(true),
      IsResizable(true)
{}

Vector2 IScreenController::GetScreenResolution() const {
    return screenResolution;
}

Vector2 IScreenController::GetWindowSize() const {
    return windowSize;
}

float IScreenController::GetTargetFrameRate() const {
    return targetFrameRate;
}

Color IScreenController::GetBackgroundColor() const {
    return backgroundColor;
}

WindowMode IScreenController::GetWindowMode() const {
    return windowMode;
}

WindowRescaleMode IScreenController::GetRescaleMode() const {
    return rescaleMode;
}

bool IScreenController::GetShowBorder() const {
    return ShowBorder;
}

bool IScreenController::GetIsResizable() const {
    return IsResizable;
}

void IScreenController::SetTargetFrameRate(float frameRate) {
    targetFrameRate = frameRate;
}

void IScreenController::SetBackgroundColor(const Color& color) {
    backgroundColor = color;
}
