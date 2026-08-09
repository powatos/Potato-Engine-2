/** @file "BoxElement.hpp" */
#pragma once
#include "WidgetElement.hpp"
#include "Util/Color.hpp"


struct BoxElement : public WidgetElement
{
    DEFINE_RENDER_PASSTHROUGH

public:
    BoxElement(const std::string& UID);

    Color FillColor;
    Color BorderColor;

    int BorderWeight;

};
