/** @file UIElement.hpp */
#pragma once

#include <set>

#include "Core/HTreeNode.h"
#include "____LLUIRenderer.hpp"
#include "Core/IWindowController.hpp"
#include "Util/UIVector.hpp"
#include "Util/Vector2.hpp"

class UIElement;

using UIHierarchy = HTreeNode<UIElement>;

/**
 * @brief Wrapper for elements that can be rendered on screen
 * @details Used for scaling size, position, offsets, and visibility parameters across different
 * types of UI targets
 */
class UIElement
{
public:
    virtual void ___Render_Passthrough(___LLUIRenderer* r, UIVector po, UIVector so) {};

    /**
     * @brief Constructs UI element
     * @warning Never put UI initialization logic in the constructor! @sa Setup()
     */
    UIElement(const std::string& UID);
    virtual ~UIElement() = default;

    /**
     * @brief Place initialization and UI setup logic in this function
     * @details This function runs automatically after the UI object is constructed
     */
    virtual void Setup();

    const std::string& GetUID() const; /**< @brief Gets unique ID of widget @returns UID */

    virtual UIVector GetScreenSize() const; ///< @brief Gets size of UI @returns Vector with x and y size components
    virtual void SetScreenSize(const UIVector& size); ///< @brief Sets size of UI @param size Vector with x and y size components

    virtual UIVector GetScreenPosition() const; ///< @brief Gets screen position of UI @returns Vector with x and y screen positions
    virtual void SetScreenPosition(const UIVector& ScreenPosition); ///< @brief Sets screen position of UI @param ScreenPosition vector with x and y screen positions
    virtual void AddScreenOffset(const UIVector& offset); ///< @brief Adds offset to UI position @param offset Vector with offset direction

    virtual bool isVisible() const; ///< @brief Checks if UI is visible @returns true if visible
    virtual void SetVisibility(bool visibility); ///< @brief Sets visibility of UI @param visibility Visibility to set
    virtual void ToggleVisibility(); ///< @brief Toggles visibility of UI

    void SetZIndex(int zIndex);
    int GetZIndex() const;

    Vector2 GetTruePosition() const; ///< @brief Gets true pixel-position on screen
    Vector2 GetTrueSize() const; ///< @brief Gets true pixel-size on screen

    UIHierarchy* ___hierarchyRef;
private:

    const std::string UID;
    int ZIndex;

    UIVector ScreenSize;
    UIVector ScreenPosition;
    bool Visible;

};