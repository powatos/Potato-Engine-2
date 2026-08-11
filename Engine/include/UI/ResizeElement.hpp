/** @file "ResizeElement.hpp" */
#pragma once
#include "WidgetElement.hpp"
#include "Core/Tickable.hpp"

/**
 * @brief Element that dynamically resizes an element based on its siblings' sizes
 * @details The element checks for the smallest box containing all its siblings and sets the size of ResizeTarget to be that size.
 * ResizeTarget does not necessarily have to be a sibling; it is ignored in box calculation if so.
 */
struct ResizeElement : public WidgetElement, public Tickable
{

    ResizeElement(const std::string& UID);
    virtual ~ResizeElement() = default;

public:
    virtual void TickPostPhysics(float dt) override;

    UIElement* ResizeTarget; ///< @brief Element to resize

    int padding; ///< @brief Padding on each side to apply after resizing

};
