/** @file "GameplayUtil.hpp" */
#pragma once
#include "Vector2.hpp"

/**
 * @brief Helper class with useful utility functions
 * @details This class is uninstantiable. All helpers are `static`
 */
class GameplayUtil {
    GameplayUtil() = delete;
public:
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
    static float RandomFloat(float min, float max);

    /**
     * @brief Generates and returns a random boolean
     */
    static int RandomBool();

    /**
     * @brief Linearly interpolates between two floats
     * @param start Starting value
     * @param end Ending value
     * @param alpha Alpha to lerp toward
     */
    static float LerpF(float start, float end, float alpha);

    /**
     * @brief Generates and returns a perlin noise coordinate
     * @param input Coordinate input
     */
    static float PerlinNoise(const Vector2& input, float frequency = 1.f, uint32_t seed = 0);

};