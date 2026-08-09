/** @file Widget.cpp */
#include <ranges>

#include "UI/WidgetElement.hpp"

#include "Debug/Log.hpp"

#include "UI/Widget.hpp"

Widget::Widget(const std::string& UID) : UIElement(UID) {

    SetScreenPosition(UIVector());
    SetScreenSize(UIVector(0.f, 0.f, 10.f, 10.f));

}


Widget::~Widget() {


}