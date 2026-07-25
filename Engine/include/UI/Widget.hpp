/** @file Widget.hpp */
#pragma once

#include <unordered_map>
#include <vector>

#include "UI/UIElement.hpp"

struct WidgetElement;

/**
 * @brief UI element that can be displayed on screen
 */
class Widget : public UIElement
{
public:
    /**
     * @brief Constructs widget
     * @details Put all class level functionality here (member initializations, 
     * default values, etc.)
     * @param UID Unique ID that identifies this widget
     * @warning Never construct widgets manually. @sa HUDController::AddWidget
     */
    Widget(std::string UID);
    virtual ~Widget();

    /** @brief Gets unique ID of widget @returns UID */
    std::string GetUID() const; 

    /**
     * @brief Creates and adds WidgetElement to widget screen
     * * @tparam ElemType Element class to add
     * @param name Unique name of element
     * @returns `WidgetElement*` that was added
     */
    template<std::derived_from<WidgetElement> ElemType>
    ElemType* AddElement(std::string name);

    /** @brief Gets map of all elements @returns `[name, WidgetElement*]` map */
    const std::unordered_map<std::string, WidgetElement*>& GetAllElements() const;
    /** @brief Gets specific element by name @param name name of element @returns element */
    WidgetElement* GetElement(std::string name);

private:

    std::unordered_map<std::string, WidgetElement*> Elements;

private:
    const std::string UID;

};

template<std::derived_from<WidgetElement> ElemType>
ElemType* Widget::AddElement(std::string name) {
    ElemType* elem = new ElemType();
    Elements[name] = elem;

    return elem;
}