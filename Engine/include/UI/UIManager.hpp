/** @file UIManager.hpp */
#pragma once

#include <vector>

#include "Core/Hierarchy.h"
#include "Core/EngineSubsystem.hpp"
#include "Core/Singleton.hpp"

#include "Widget.hpp"

using UIHierarchy = Hierarchy<UIElement>;

class UIManager : public EngineSubsystem<UIManager>
{
    ENGINE_SUBSYSTEM(UIManager)
    
public:
    virtual void Resolve() noexcept override;

    UIHierarchy* RootWidgetH;

    /**
     * @brief Construct and add UI parented to another UI onto the screen
     * @note Widgets may only be parented under other widgets, but WidgetElements may follow any hierarchical order
     * * @tparam UIClass Widget or WidgetElement type to add
     * @param UID Unique ID to identify UI
     * @param parent UI to parent this UI to
     * @returns UI that was added
     */
    template<std::derived_from<UIElement> UIClass, std::derived_from<UIElement> Parent = UIElement>
    UIClass* AddUI(const std::string& UID, Parent* parent);
    /**
     * @brief Construct and add a base widget (parented to the root widget)
     * * @tparam WidgetClass Widget type to add
     * @param UID Unique ID to identify UI
     * @returns Widget that was added
     */
    template<std::derived_from<Widget> WidgetClass>
    WidgetClass* AddUI(const std::string& UID);

    /**
     * @brief Remove a Widget from the screen
     * @param UID UID of the widget to remove
     * @note This function deletes the widget instance and render buffer. Do not use this
     * to hide the widget temporarily, or when preservation of data is vital. @sa Widget::SetVisibility
     */
    virtual void RemoveUI(const std::string& UID);

    /** @brief Gets UI by UID
     * @param UID UID
     * @param HintParent Parent of target UI; optional if unknown
     * @returns UI matching name
     */
    virtual UIElement* GetUI(const std::string& UID, const UIElement* HintParent = nullptr);

private:
    UIManager();
    ~UIManager();

    void Recurse_Add(UIElement* ui, UIElement* parent, UIHierarchy* currentLevel);
    void Recurse_Remove(const std::string& UID, UIHierarchy* currentLevel);
    UIElement* Recurse_Get(const std::string& UID, const UIHierarchy* currentLevel);
};

template<std::derived_from<UIElement> UIClass, std::derived_from<UIElement> Parent>
UIClass* UIManager::AddUI(const std::string& UID, Parent* parent) {
    static_assert(
        !std::derived_from<UIClass, Widget> ||
        std::derived_from<Parent, Widget>,
        "Widgets may only be parented to other widgets!"
    ); // keep static assertion instead of concept for readability

    UIClass* ui = new UIClass(UID);

    Recurse_Add(ui, parent, RootWidgetH);
    ui->Setup();

    return ui;
}

template<std::derived_from<Widget> WidgetClass>
WidgetClass* UIManager::AddUI(const std::string& UID) {
    WidgetClass* ui = AddUI<WidgetClass>(UID, static_cast<Widget*>(RootWidgetH->Object));
    return ui;
}
