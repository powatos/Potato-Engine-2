/** @file UIElement.hpp */
#pragma once

#include "Util/Vector2.hpp"

#include "Core/Tickable.hpp"

/**
 * @brief Wrapper for elements that can be rendered on screen
 * @details Used for scaling size, position, offsets, and visibility parameters across different
 * types of UI targets
 */
class UIElement : public Tickable
{
public:
    virtual ~UIElement() = default;
    
    Vector2 GetScreenSize() const; ///< @brief Gets size of UI @returns Vector with x and y size components
    void SetScreenSize(const Vector2& size); ///< @brief Sets size of UI @param size Vector with x and y size components

    Vector2 GetScreenPosition() const; ///< @brief Gets screen position of UI @returns Vector with x and y screen positions
    void SetScreenPosition(const Vector2& ScreenPosition); ///< @brief Sets screen position of UI @param ScreenPosition vector with x and y screen positions
    void AddScreenOffset(const Vector2& offset); ///< @brief Adds offset to UI position @param offset Vector with offset direction

    bool isVisible() const; ///< @brief Checks if UI is visible @returns true if visible
    void SetVisibility(bool visibility); ///< @brief Sets visibility of UI @param visibility Visibility to set
    void ToggleVisibility(); ///< @brief Toggles visibility of UI

private:
    Vector2 ScreenSize;
    Vector2 ScreenPosition;
    bool Visible;

};