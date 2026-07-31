/** @file Texture.hpp */
#pragma once

#include "Core/Asset.hpp"

#include <string>
#include <vector>

#include "Util/Color.hpp"

/**
 * @brief Texture struct for representing an actor texture
 */
struct Texture : public Asset
{

    void* sdl_surface;
    void* sdl_texture;

    Color GetKeyColor();
    void SetKeyColor(Color color);

    Texture(FilePath path, void* surface, void* texture);

    Texture(Texture&& other) noexcept;

    ~Texture();

private:
    Color keyColor; // 0x0 alpha means do not use color key

};