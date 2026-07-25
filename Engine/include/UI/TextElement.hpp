/** @file TextElement.hpp */
#pragma once

#include "WidgetElement.hpp"

/**
 * @brief Element that displays text on a widget
 */
struct TextElement : public WidgetElement
{
    virtual const char* TYPE() const override { return "TextElement"; }

    TextElement() = default;

    /**
     * @brief Text to display
     */
    std::string field;

};