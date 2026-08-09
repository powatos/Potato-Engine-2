/** @file Vector2.hpp */
#pragma once

#include <cmath>
#include <format>
#include <string>

static constexpr double PI = 3.14159265358979323846;

/**
 * @brief Standard 2-dimensional vector
 */
struct Vector2
{
    
    float x; /**< @brief X Component */
    float y; /**< @brief Y Component */

    /** @brief Constructs zero vector */
    constexpr Vector2() : x{0.f}, y{0.f} {}
    /** @brief Constructs custom vector @param x X @param y Y */
    constexpr Vector2(const float x, const float y) : x(x), y(y) {}
    /** @brief Constructs vector from copy @param vec Vector to copy */
    constexpr Vector2(const Vector2& vec) : x(vec.x), y(vec.y) {} 

    /**
     * @brief unit vector pointing up
     */
    inline static constexpr Vector2 Up() { return Vector2{0.f,1.f}; }
    /**
     * @brief unit vector pointing right
     */
    inline static constexpr Vector2 Right() { return Vector2{1.f,0.f}; }

    /**
     * @brief addition of two vectors
     */
    inline Vector2 operator +(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }
    /** @brief Adds to vector */
    inline void operator +=(const Vector2& other) {
        x += other.x;
        y += other.y;
    }
    /**
     * @brief subtraction of two vectors
     */
    inline Vector2 operator -(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }
    /** @brief Subtracts from vector */
    inline void operator -=(const Vector2& other) {
        x -= other.x;
        y -= other.y;
    }
    /**
     * @brief amplification by a scalar
     */
    inline Vector2 operator *(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    /**
     * @brief hadamard product of two vectors
     */
    inline Vector2 operator *(const Vector2& other) const {
        return Vector2(x*other.x, y*other.y);
    }

    /** @brief Hadamard multiplies to vector */
    inline void operator *=(const Vector2& other) {
        x *= other.x;
        y *= other.y;
    }

    /**
     * @brief division by a scalar
     */
    inline Vector2 operator /(float scalar) const {
        return Vector2(x / scalar, y / scalar);
    }

    /**
     * @brief hadamard quotient of two vectors
     */
    inline Vector2 operator /(const Vector2& other) const {
        return Vector2(x / other.x, y / other.y);
    }

    /** @brief Hadamard divides to vector (safe with zero division) */
    inline Vector2 operator /=(const Vector2& other) {
        return Vector2(other.x != 0.f ? x/other.x : 0.f, other.y != 0.f ? y/other.y : 0.f);
    }

    /**
     * @brief negation of vector
     */
    inline Vector2 operator -() const {
        return Vector2(-x, -y);
    }

    /**
     * @brief swizzled vector (x,y)->(y,x)
     */
    inline Vector2 Swizzled() const {
        return Vector2(y, x);
    }

    /**
     * @brief [dot product](https://en.wikipedia.org/wiki/Euclidean_vector#Dot_product) of two vectors
     */
    inline float Dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }

    /**
     * @brief [magnitude](https://en.wikipedia.org/wiki/Euclidean_vector#Length) of vector
     */
    inline float Magnitude() const {
        return std::hypotf(x,y);
    }

    /**
     * @brief [unit](https://en.wikipedia.org/wiki/Euclidean_vector#Unit_vector) vector
     */
    inline Vector2 Normalized() const {
        float mag = Magnitude();
        if (mag == 0.f) return Vector2(0.f, 0.f);
        return Vector2(x / mag, y / mag);
    }

    /**
     * @brief distance to another vector
     */
    inline float Distance(const Vector2& other) const {
        const float dx = x-other.x;
        const float dy = y-other.y;

        return std::sqrt(dx * dx + dy * dy );
    }

    /**
     * @brief the square of the distance to another vector
     */
    inline float SquaredDistance(const Vector2& other) const {
        const float dx = x-other.x;
        const float dy = y-other.y;

        return  dx * dx + dy * dy;
    }

    /**
     * @brief a rotated vector @param rotation rotation in degrees
     */
    inline Vector2 Rotate(float rotation) const {
        const float sine = std::sin(rotation * PI/180);
        const float cosine = std::cos(rotation * PI/180);

        return Vector2(
            x * cosine - y * sine,
            x * sine + y * cosine
        );
    }

    /**
     * @brief the angle of the vector from standard position in degrees
     */
    inline float Angle() const {
        return 180/PI * std::atan2(y, x);
    }

    /**
     * @brief string representation of vector
     */
    inline std::string ToString() const {
        return std::format("({}, {})", static_cast<int>(x), static_cast<int>(y));
    }

    /**
     * @brief string representation of vector using float
     */
    inline std::string ToStringF() const {
        return std::format("({:.3f}, {:.3f})", x, y);
    }

};

inline Vector2 operator *(float scalar, const Vector2& vec) {
    return vec * scalar;
}
