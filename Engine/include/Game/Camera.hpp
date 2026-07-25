/** @file Camera.hpp */
#pragma once

#include "Actor.hpp"

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

protected:
};