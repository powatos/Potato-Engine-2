/** @file HUDController.hpp */
#pragma once

#include <concepts>

class Widget;

/**
 * @brief Manages global HUD
 */
class IHUDController
{
public:
    virtual ~IHUDController() = default;

    /**
     * @brief Remove a Widget from the screen
     * @param UID UID of the widget to remove
     * @note This function deletes the widget instance and render buffer. Do not use this
     * to hide the widget temporarily, or when preservation of data is vital. @sa Widget::SetVisibility
     */
    virtual void RemoveWidget(std::string UID) = 0;
    /**
     * @brief Construct and add widget to the screen
     * * @tparam WidgetClass Specific class of widget to add
     * @param UID Unique ID to identify widget
     * @returns Widget that was added 
     */
    template<std::derived_from<Widget> WidgetClass>
    WidgetClass* AddWidget(std::string UID);
    /** @brief Gets widget by UID @param UID UID @returns Widget matching name */
    virtual Widget* GetWidget(std::string UID) const = 0;
    
protected:
    /** @internal */
    virtual void RegisterWidget(Widget* widget) = 0;

};

template <std::derived_from<Widget> WidgetClass>
WidgetClass* IHUDController::AddWidget(std::string UID) {
    WidgetClass* widget = new WidgetClass(UID);
    RegisterWidget(widget);
    return widget;
}