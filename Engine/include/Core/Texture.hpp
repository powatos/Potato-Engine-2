/** @file Texture.hpp */
#pragma once

#include "Core/Asset.hpp"

#include <string>
#include <vector>

#include "Util/Color.hpp"

/**
 * @brief Scaling type for textures
 */
enum class TextureScaleMode
{
    None = -1, ///< No texture scaling
    Nearest, ///< Nearest pixel sampling
    Linear, ///< Linear filtering
    Pixel ///< Nearest pixel sampling for pixel art
};

/**
 * @brief Struct representing an actor texture
 * @copydetails Asset
 */
struct Texture : public Asset
{
    Texture(FilePath path, void* surface, void* texture);
    Texture(Texture&& other) noexcept;

    ~Texture();

    Color GetKeyColor() const;
    void SetKeyColor(Color color);

    TextureScaleMode GetScaleMode() const;
    void SetScaleMode(TextureScaleMode mode);

    void* ___Get_Surface() const;
    void* ___Get_Texture() const;

private:
    Color keyColor; // 0x0 alpha means do not use color key
    TextureScaleMode scaleMode;

    void* sdl_surface;
    void* sdl_texture;

};