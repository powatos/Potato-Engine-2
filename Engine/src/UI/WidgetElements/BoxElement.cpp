/** @file "BoxElement.cpp" */

#include "UI/BoxElement.hpp"

#include <SDL3/SDL_rect.h>

BoxElement::BoxElement(const std::string& UID) :
    WidgetElement(UID),
    FillColor(Color::WHITE()),
    BorderColor(Color::BLACK()),
    BorderWeight(0)
{

}

