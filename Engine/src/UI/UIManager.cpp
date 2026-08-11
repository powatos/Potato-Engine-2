/** @file UIManager.cpp */

#include "UI/Widget.hpp"

#include "Debug/Log.hpp"

#include "UI/UIManager.hpp"

UIManager::UIManager() {
    LOG(LogType::VITAL, "UIManager constructed");

    RootWidget = new Widget("RootWidget");

    RootWidget_H = new HTreeNode<UIElement>();
    RootWidget_H->Object = RootWidget;
    RootWidget_H->Object->SetScreenSize(UIVector(1.f, 1.f, 0.f, 0.f));

}

void UIManager::RemoveUI(const std::string& UID) {
    Recurse_Remove(UID, RootWidget_H);
}

UIElement* UIManager::GetUI(const std::string& UID, const UIElement* HintParent) {

    // TODO: implement hint
    // if (HintParent != nullptr) {
    //     return Recurse_Get(UID, HintParent);
    // }

    return Recurse_Get(UID, RootWidget_H);
}

void UIManager::___SetZIndex(UIElement* ui, int zIndex) const {
    ui->___hierarchyRef->Parent->ReorderChild(ui, zOrderingPred);
}

void UIManager::Resolve() noexcept {
    LOG(LogType::VITAL, "Resolving UIManager");

    delete RootWidget_H;
}

#pragma region Recursive helpers

void UIManager::Recurse_Add(UIElement* ui, UIElement* parent, UIHierarchy* currentLevel) {
    if (parent == nullptr) {
        RootWidget_H->AddChild(ui, &ui->___hierarchyRef, zOrderingPred);
        return;
    }

    if ( currentLevel->Object == parent ) {
        currentLevel->AddChild(ui, &ui->___hierarchyRef, zOrderingPred);
        return;
    }

    for (UIHierarchy* child : currentLevel->Children) {
        Recurse_Add(ui, parent, child);
    }
}

void UIManager::Recurse_Remove(const std::string& UID, UIHierarchy* currentLevel) {

    if (currentLevel->Object->GetUID() == UID) {
        delete currentLevel;
        return;
    }

    for (UIHierarchy* child : currentLevel->Children) {
        Recurse_Remove(UID, child);
    }

}
UIElement* UIManager::Recurse_Get(const std::string& UID, const UIHierarchy* currentLevel) {
    if (currentLevel->Object->GetUID() == UID) {
        return currentLevel->Object;
    }

    for (UIHierarchy* child : currentLevel->Children) {
        UIElement* ui = Recurse_Get(UID, child);
        if (ui != nullptr) { return ui; }
    }

    return nullptr;
}

bool UIManager::zOrderingPred(const UIElement* sibling, const UIElement* insertingChild) {
    return insertingChild->GetZIndex() < sibling->GetZIndex();
}

#pragma endregion

UIManager::~UIManager() {
}
