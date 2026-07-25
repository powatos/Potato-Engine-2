/** @file Raycast.hpp */
#pragma once

#include <unordered_set>

#include "Game/Actor.hpp"
#include "HitResult.hpp"

class Block;

/**
 * @enum RaycastOverrideType
 * @brief Types of overrides that can be used to filter a list for a raycast
 */
enum class RaycastOverrideType
{
    NoOverride, ///< List isn't used in raycast
    Exclude, ///< Exclude list members from triggering raycast
    Include ///< Include only list members from triggering raycast
};

/**
 * @brief Settings for a raycast
 */
struct RaycastSettings
{
    std::unordered_set<Actor*> OverrideActorsList{}; ///< Actors to override raycast behaviour on
    RaycastOverrideType OverrideActorsType = RaycastOverrideType::NoOverride; ///< Type of override to use for OverrideActorsList
};

/**
 * @brief Struct used to cast a ray in the world
 * @details Construct to initialize the raycast and call Cast() to cast
 */
struct Raycast
{
    bool showDebug; ///< @brief Displays a ray trail on screen when the ray is cast
    double debugDuration; ///< @brief Duration of the trail if showDebug is set true
    Vector2 origin;
    RaycastSettings Settings;

    /**
     * @brief Constructs a raycast with empty settings
     * @details Ensure the raycast is set up before casting
     */
    Raycast();
    /**
     * @brief Constructs a raycast
     * @param origin Origin of the raycast
     * @param ray Vector representing the ray; magnitude represents max length
     */
    Raycast(const Vector2& origin, const Vector2& ray, RaycastSettings Settings = RaycastSettings{});
    
    /**
     * @brief Casts the ray
     * @param outHit HitResult reference that contains information about the raycast
     * @returns the trail that was spawned if showDebug was set true; `nullptr` if set false
     */
    Block* Cast(HitResult& outHit);

    /**
     * @brief Sets the ray of the raycast
     * @param newRay Vector representing the ray; magnitude represents max length
     */
    void SetRay(const Vector2& newRay);

private:
    float rayLength;
    Vector2 ray;

    void ComputeRaycast(Actor* actor, HitResult& outHit);
};