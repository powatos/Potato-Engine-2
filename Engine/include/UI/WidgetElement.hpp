/** @file WidgetElement.hpp */
#pragma once

#include "UI/UIElement.hpp"

/**
 * @brief Wrapper for specialized sub-element that can be rendered by a Widget
 */
struct WidgetElement : public UIElement
{
    using UIElement::UIElement;

    virtual ~WidgetElement() = default;

protected:

};