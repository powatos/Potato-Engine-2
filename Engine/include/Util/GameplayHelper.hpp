/** @file GameplayHelper.hpp */
#pragma once

#include "Util/Vector2.hpp"

class Camera;
class Actor;

/**
 * @brief Helper class with useful runtime functions
 * @details This class is uninstantiable. All helpers are `static`
 */
class GameplayHelper {
    GameplayHelper() = delete;

public:
    /**
     * @brief Converts world position to screen position
     * @details World origin is game-relative (0,0). Screen origin is at the top left corner
     * of the terminal window. Use this function to convert world coordinates to screen-relative
     * @param worldPos position to convert
     * @returns converted screen position as vector
     */
    static Vector2 VecToScreenVec(const Vector2& worldPos);
    /**
     * @brief Checks if a position is visible in the view of a camera object
     * @param pos Position to check
     * @param camera Camera to check for visibility
     * @returns true if is in frame
     */
    /**
     * @brief Gets list of overlapping actors
     * @param actor actor to check for overlaps
     * @returns vector of actor pointers that are overlapping
     */
    static std::vector<Actor*> GetOverlappingActors(const Actor* actor);
    /**
     * @brief Checks if two actors are overlapping
     * @param actor1 first actor
     * @param actor2 second actor
     * @returns true if overlapping
     */
    static bool IsActorOverlapping(const Actor* actor1, const Actor* actor2);

};