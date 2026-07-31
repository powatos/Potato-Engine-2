/** @file Texture.cpp */

#include "Core/Texture.hpp"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>

#include "Core/IScreenController.hpp"
#include "Core/PotatoEngine.hpp"
#include "Debug/Log.hpp"

Texture::Texture(FilePath path, void* surface, void* texture) :
    Asset(path),
    sdl_surface(surface),
    sdl_texture(texture)
{}

Texture::Texture(Texture&& other) noexcept :
    Asset(std::move(other)),
    sdl_surface(other.sdl_surface),
    sdl_texture(other.sdl_texture)
{
    other.sdl_surface = nullptr;
    other.sdl_texture = nullptr;
}


Color Texture::GetKeyColor() {
    return keyColor;
}


void Texture::SetKeyColor(Color color) {

    if (sdl_texture == nullptr) {
        LOG(LogType::ERROR, "Unallocated texture member while trying to set texture key color");
        return;
    }

    keyColor = color;

    SDL_Surface* surface = static_cast<SDL_Surface*>(sdl_surface);

    if (keyColor.A == 0x00) {
        SDL_SetSurfaceColorKey(
            surface,
            false,
            0
        );
    } else {

        const SDL_PixelFormatDetails* format = SDL_GetPixelFormatDetails(surface->format);
        const SDL_Palette* palette = SDL_GetSurfacePalette(surface);
        const Uint32 key = SDL_MapRGB(format, palette, color.R, color.G, color.B);

        SDL_SetSurfaceColorKey(
            static_cast<SDL_Surface*>(sdl_surface),
            true,
            key
        );

    }

    SDL_DestroyTexture(static_cast<SDL_Texture*>(sdl_texture));

    SDL_Renderer* renderer = static_cast<SDL_Renderer*>(PotatoEngine::Get().GetScreenController()->RequestRenderingContext());

    sdl_texture = SDL_CreateTextureFromSurface(renderer, surface);


}

Texture::~Texture() {
    SDL_DestroyTexture(static_cast<SDL_Texture*>(sdl_texture));
    SDL_DestroySurface(static_cast<SDL_Surface*>(sdl_surface));
}