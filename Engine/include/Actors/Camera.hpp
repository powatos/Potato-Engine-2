/** @file Camera.hpp */
#pragma once

#include "Game/Actor.hpp"

/**
 * @brief Camera wrapper for rendering
 * @details This class is a wrapper used for rendering logic. Assigned to controller
 * while @ref PotatoEngine::LoadSubclasses "loading subclasses". A player cannot render
 * the level without a camera assigned to the controller
 */
class Camera : public Actor
{

public:
    Camera();
    ~Camera();

    /**
     * @brief Sets zoom of camera
     * @param zoom Camera zoom
     */
    void SetZoom(float zoom);

    /**
     * @brief Gets zoom of camera
     * @return Camera zoom
     */
    float GetZoom() const;

    /**
     * @brief Sets view height of camera
     * @param height Camera view height
     */
    void SetViewHeight(float height);

    /**
     * @brief Gets view height of camera
     * @return Camera view height
     */
    float GetViewHeight() const;

    virtual void TickPostUpdate(float dt) override;

protected:

    float Zoom;
    float ViewHeight;
};


