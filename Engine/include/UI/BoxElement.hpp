/** @file "BoxElement.hpp" */
#pragma once
#include "WidgetElement.hpp"
#include "Util/Color.hpp"

/**
 * @brief Element that displays a box on a widget
 */
struct BoxElement : public WidgetElement
{
    DEFINE_RENDER_PASSTHROUGH

public:
    BoxElement(const std::string& UID);

    Color FillColor; ///< @brief Fill color of the box @note Set the alpha to control transparency
    Color BorderColor; ///< @brief Border color of the box

    int BorderWeight; ///< @brief Pixel-width of the border @note Set to 0 to disable border

};
