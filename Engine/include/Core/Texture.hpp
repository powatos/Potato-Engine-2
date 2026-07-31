/** @file Texture.hpp */
#pragma once

#include "Core/Asset.hpp"

#include <string>
#include <vector>

#include "Util/Color.hpp"

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

    void* ___Get_Surface() const;
    void* ___Get_Texture() const;

private:
    Color keyColor; // 0x0 alpha means do not use color key

    void* sdl_surface;
    void* sdl_texture;

};