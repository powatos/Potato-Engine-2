/** @file HitResult.hpp */
#pragma once

#include "Util/Vector2.hpp"

class Actor;

/**
 * @brief Struct with information about a hit between two actors
 * @details This struct is used to pass information after physics collisions, raycasts, etc.
 * @note Check against nullptr `hitActor`s for empty hits
 */
struct HitResult
{
    Actor* hitActor;
    Vector2 hitPosition;
    Vector2 hitOverlap;
    Vector2 hitNormal;

    float distance;
    float ray_param;

    HitResult() : 
        hitActor(nullptr),
        hitPosition(),
        hitOverlap(),
        hitNormal(),
        distance(0.f),
        ray_param(0.f)
    {}

    HitResult(Actor* hitActor, 
        Vector2 hitPosition, 
        Vector2 hitOverlap, 
        Vector2 hitNormal
    ) : 
        hitActor(hitActor), 
        hitPosition(hitPosition), 
        hitOverlap(hitOverlap), 
        hitNormal(hitNormal) 
    {}

};