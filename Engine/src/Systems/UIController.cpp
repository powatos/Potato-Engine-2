/** @file UIController.cpp */

#include "OutputManager.hpp"
#include "UI/Widget.hpp"

#include "Debug/Log.hpp"

#include "UIController.hpp"
UIController::UIController() {
    LOG(LogType::VITAL, "UIController constructed");

}

void UIController::RegisterWidget(Widget* widget) {

    OutputManager::Get()->RegisterWidget(widget);
    ActiveWidgets.push_back(widget);

}

void UIController::RemoveWidget(std::string UID) {
    OutputManager::Get()->RemoveWidget(UID);

    Widget* w = GetWidget(UID);
    std::erase(ActiveWidgets, w);
    delete w;
}

Widget* UIController::GetWidget(std::string UID) const {
    
    auto it = std::ranges::find_if(ActiveWidgets, [UID](Widget* widget){
        return widget->GetUID() == UID;
    });

    if (it != ActiveWidgets.end()) { return *it; }

    return nullptr;

}

const std::vector<Widget*>& UIController::GetActiveWidgets() {
    return ActiveWidgets;
}

void UIController::Resolve() noexcept {
    LOG(LogType::VITAL, "Resolving UIController");

    for (Widget* widget : ActiveWidgets) {
        delete widget;
    }

}

UIController::~UIController() {
}

