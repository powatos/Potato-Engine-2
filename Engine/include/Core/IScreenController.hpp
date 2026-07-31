/** @file "IScreenController.hpp" */
#pragma once
#include "Util/Color.hpp"
#include "Util/Vector2.hpp"

/**
 * @brief Enum representing window border mode
 */
enum class WindowMode
{
    Windowed,
    Fullscreen
};

/**
 * @brief
 */
enum class WindowRescaleMode
{
    None, ///< No rescale
    Stretch, ///< Content is stretched to resolution
    Letterbox ///< Content fits resolution aspect ratio with background color borders
};

/**
 * @brief Interface to manage screen settings and operations
 */
class IScreenController {
protected:

    Vector2 screenResolution;
    Vector2 windowSize;

    float targetFrameRate;

    Color backgroundColor;

    WindowMode windowMode;
    WindowRescaleMode rescaleMode;

    bool ShowBorder;
    bool IsResizable;

    IScreenController();
    virtual ~IScreenController() = default;

public:

    virtual void* RequestRenderingContext() const = 0;

    /**
     * @brief Gets resolution of screen
     * @returns resolution
     */
    Vector2 GetScreenResolution() const;

    /**
     * @brief Sets resolution of screen
     * @param resolution new resolution
     */
    virtual void SetScreenResolution(const Vector2& resolution) = 0;

    /**
     * @brief Gets pixel size of window
     * @returns window size
     */
    Vector2 GetWindowSize() const;

    /**
     * @brief Sets pixel size of window
     * @param size new window size
     */
    virtual void SetWindowSize(const Vector2& size) = 0;

    /**
     * @brief Gets target frame rate for rendering
     * @returns target frame rate
     */
    float GetTargetFrameRate() const;

    /**
     * @brief Sets target frame rate for rendering
     * @param frameRate new target frame rate
     */
    void SetTargetFrameRate(float frameRate);

    /**
     * @brief Gets default world render color
     * @returns background color
     */
    Color GetBackgroundColor() const;

    /**
     * @brief Sets default world render color
     * @param color new background color
     */
    void SetBackgroundColor(const Color& color);

    /**
     * @brief Gets current window mode
     * @returns window mode
     */
    WindowMode GetWindowMode() const;

    /**
     * @brief Sets current window mode
     * @param mode new window mode
     */
    virtual void SetWindowMode(WindowMode mode) = 0;

    /**
     * @brief Gets current window rescale mode
     * @returns rescale mode
     */
    WindowRescaleMode GetRescaleMode() const;

    /**
     * @brief Sets current window rescale mode
     * @param mode new rescale mode
     */
    virtual void SetRescaleMode(WindowRescaleMode mode) = 0;

    /**
     * @brief Gets whether the window border is shown
     * @returns window border visibility
     */
    bool GetShowBorder() const;

    /**
     * @brief Sets whether the window border is shown
     * @param show window border visibility
     */
    virtual void SetShowBorder(bool show) = 0;

    /**
     * @brief Gets whether the window is resizable
     * @returns window resizable state
     */
    bool GetIsResizable() const;

    /**
     * @brief Sets whether the window is resizable
     * @param isResizable window resizable state
     */
    virtual void SetIsResizable(bool isResizable) = 0;

};