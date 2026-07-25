/** @file GameplayHelper.hpp */
#pragma once

class Camera;
struct Vector2;
class Actor;

/**
 * @brief Helper class with useful logic and shorthand functions
 * @details This class is uninstantiable. All helpers are `static`
 */
class GameplayHelper {
    GameplayHelper() = default;

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
    // static bool IsPositionInCameraFrame(const Vector2& pos, Camera* camera);
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

    /**
     * @brief Generates and returns a random integer within a range
     * @param min minimum value (inclusive)
     * @param max maximum value (inclusive)
     */
    static int RandomInt(int min, int max);

    /**
     * @brief Generates and returns a random float within a range
     * @param min minimum value (inclusive)
     * @param max maximum value (exclusive)
     */
    static int RandomFloat(float min, float max);

    /**
     * @brief Generates and returns a random boolean
     */
    static int RandomBool();
};