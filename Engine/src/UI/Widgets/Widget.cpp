/** @file Widget.cpp */

#include "UI/WidgetElement.hpp"

#include "Debug/Log.hpp"

#include "UI/Widget.hpp"

Widget::Widget(std::string UID) : UID(UID) {
    SetScreenPosition(Vector2(0.f,0.f));
    SetScreenSize(Vector2(16.f, 4.f));
    SetVisibility(true);

}

std::string Widget::GetUID() const {
    return UID;
}

const std::unordered_map<std::string, WidgetElement*>& Widget::GetAllElements() const {
    return Elements;
}
WidgetElement* Widget::GetElement(std::string name) {
    if (Elements.find(name) != Elements.end()) {
        return Elements.at(name);
    }

    return nullptr;
}

Widget::~Widget() {

    for (auto it = Elements.begin(); it != Elements.end(); ) {
        delete it->second;
        it = Elements.erase(it);
    }

}