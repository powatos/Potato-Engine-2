/** @file Color.hpp */

#pragma once

#include <cstdint>

struct Color
{
    std::uint8_t R; ///< @brief Red component
    std::uint8_t G; ///< @brief Green component
    std::uint8_t B; ///< @brief Blue component
    std::uint8_t A; ///< @brief Alpha component

    /** @brief Constructs zero color with full alpha */
    constexpr Color() : R(0x00), G(0x00), B(0x00), A(0xff) {}
    /** @brief Constructs custom color @param R Red @param G Green @param B Blue */
    explicit constexpr Color(const std::uint8_t R, std::uint8_t G, std::uint8_t B) : R(R), G(G), B(B), A(0xff) {}
    /** @brief Constructs custom color with alpha @param R Red @param G Green @param B Blue @param A Alpha */
    explicit constexpr Color(const std::uint8_t R, std::uint8_t G, std::uint8_t B, std::uint8_t A) : R(R), G(G), B(B), A(A) {}
    /** @brief Constructs custom color from raw argb representation @param raw Raw color */
    explicit constexpr Color(const std::uint32_t raw) : R((raw >> 16) & 0xff ), G((raw >> 16) & 0xff), B((raw >> 8) & 0xff), A((raw >> 24) & 0xff) {}
    /** @brief Constructs color from copy @param col Color to copy */
    constexpr Color(const Color& col) : R(col.R), G(col.G), B(col.B), A(col.A) {}

    /**
     * @brief Full red color
     */
    inline static constexpr Color RED() { return Color(0xff, 0x00, 0x00, 0xff); }

    /**
     * @brief Full green color
     */
    inline static constexpr Color GREEN() { return Color(0x00, 0xff, 0x00, 0xff); }

    /**
     * @brief Full blue color
     */
    inline static constexpr Color BLUE() { return Color(0x00, 0x00, 0xff, 0xff); }

    /**
     * @brief Full white color
     */
    inline static constexpr Color WHITE() { return Color(0xff, 0xff, 0xff, 0xff); }

    /**
     * @brief Full black color
     */
    inline static constexpr Color BLACK() { return Color{}; }

    /**
     * @brief Full zero-ed color
     */
    inline static constexpr Color BLANK() { return Color(0x0); }

    /**
     * @brief a raw 32-bit integer representation of the color with alpha as the most significant byte
     */
    inline std::uint32_t raw_argb() const {
        return A << 24 | R << 16 | G << 8 | B;
    }

    /**
     * @brief a raw 32-bit integer representation of the color with only RGB values and a zero-ed most significant byte
     */
    inline std::uint32_t raw_rgb() const {
        return 0x00 << 24 | R << 16 | G << 8 | B;
    }

};