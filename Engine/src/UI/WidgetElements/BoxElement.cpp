/** @file "BoxElement.cpp" */

#include "UI/BoxElement.hpp"

#include <SDL3/SDL_rect.h>

#include "UI/UIManager.hpp"

REGISTER_WIDGETELEMENT(BoxElement)

BoxElement::BoxElement(const std::string& UID) :
    WidgetElement(UID),
    FillColor(Color::WHITE()),
    BorderColor(Color::BLACK()),
    BorderWeight(0)
{

}
