/** @file UIVector.hpp */
#pragma once

#include "Vector2.hpp"

/**
 * @brief Two-dimensional vector for storing UI unit data
 */
struct UIVector
{
    Vector2 scale; ///< @brief Percent of screen units
    Vector2 offset; ///< @brief Actual pixel units

    /**
     * @brief Constructs zero UIVector
     *
     */
    constexpr UIVector() : scale{}, offset{} {}
    /**
     * @brief Constructs custom UIVector from raw
     *
     */
    constexpr UIVector(const float scaleX, const float scaleY, const float offsetX, const float offsetY) :
        scale{scaleX, scaleY}, offset{offsetX, offsetY}
    {}
    /**
     * @brief Constructs custom UIVector from scale and offset vectors
     * @param scale scale vector
     * @param offset offset vector
     */
    constexpr UIVector(const Vector2& scale, const Vector2& offset) : scale{scale}, offset{offset} {}
    /**
     * @brief Constructs UIVector from copy
     * @param UIVec UIVector to copy
     */
    constexpr UIVector(const UIVector& UIVec) : scale(UIVec.scale), offset(UIVec.offset) {}

    /** @brief addition of two UIVectors */
    inline UIVector operator +(const UIVector& other) const {
        return UIVector{scale + other.scale, offset + other.offset};
    }
    /** @brief subtraction of two UIVectors */
    inline UIVector operator -(const UIVector& other) const {
        return UIVector{scale - other.scale, offset - other.offset};
    }

};
