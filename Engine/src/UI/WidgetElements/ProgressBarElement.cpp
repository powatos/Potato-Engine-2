/** @file "ProgressBarElement.cpp" */

#include "UI/ProgressBarElement.hpp"

ProgressBarElement::ProgressBarElement(const std::string& UID) : WidgetElement(UID) {
    value = 0.f;
    maxValue = 1.f;
    backgroundColor = Color::WHITE();
    barColor = Color::BLUE();
}

void ProgressBarElement::AddValue(float v) {
    value += v;

    if (value > maxValue) {
        value = maxValue;
    }
}
