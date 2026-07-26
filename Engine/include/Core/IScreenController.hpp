/** @file "IScreenController.hpp" */
#pragma once
#include "Util/Color.hpp"
#include "Util/Vector2.hpp"

/**
 * @brief Interface to manage screen settings and operations
 */
class IScreenController {
public:

    /**
     * @brief Set initial screen size for display window
     * @param size size of screen
     */
    virtual void SetScreenSize(Vector2 size);
    /**
     * @brief Get initial screen size for display window
     * @returns size of screen
     */
    virtual Vector2 GetScreenSize() const;

    /**
     * @brief Set target FPS for game
     * @param fps target FPS
     */
    virtual void SetFrameRate(float fps);
    /**
     * @brief Get target FPS for game
     * @returns target FPS
     */
    virtual float GetFrameRate() const;

    /**
     * @brief Set window resizable property
     * @param resizable whether window can resize
     */
    virtual void SetIsResizable(bool resizable);
    /**
     * @brief Checks for window resizable property
     * @returns true if window can resize
     */
    virtual bool IsResizable(bool resizable) const;

    /**
     * @brief Sets background color of the screen
     * @param color color to set
     */
    virtual void SetBackgroundColor(Color color);
    /**
     * @brief Gets background color of the screen
     * @returns Background color
     */
    virtual Color GetBackgroundColor() const;

protected:
    IScreenController();
    virtual ~IScreenController() = default;

    Vector2 ScreenSize;

    float FrameRate;
    Color BackgroundColor;

public:

protected:
    bool bIsResizable;
};