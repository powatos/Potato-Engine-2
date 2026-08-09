/** @file Widget.hpp */
#pragma once

#include "UI/UIElement.hpp"

struct WidgetElement;

/**
 * @brief UI element abstraction layer that can hold elements to display on screen
 */
class Widget : public UIElement
{
public:
    DEFINE_RENDER_PASSTHROUGH

    /**
     * @brief Constructs widget
     * @details Put all class level functionality here (member initializations, 
     * default values, etc.)
     * @param UID Unique ID that identifies this widget
     * @warning Never construct widgets manually. @sa UIManager::AddWidget
     */
    Widget(const std::string& UID);
    virtual ~Widget();

private:

};