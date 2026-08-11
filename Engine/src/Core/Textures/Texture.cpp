/** @file Texture.cpp */

#include "Core/Texture.hpp"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>

#include "Core/IWindowController.hpp"
#include "Core/PotatoEngine.hpp"
#include "Debug/Log.hpp"

Texture::Texture(const FilePath& path) :
    Asset(path)
{
    SDL_Surface* surface = SDL_LoadBMP(path.string().c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(static_cast<SDL_Renderer*>(PotatoEngine::GetWindowController()->RequestRenderingContext()), surface);

    sdl_surface = surface;
    sdl_texture = texture;

    SetScaleMode(TextureScaleMode::Nearest);
    SetKeyColor(Color::BLANK());
}

Texture::Texture(Texture&& other) noexcept :
    Asset(std::move(other)),
    scaleMode(other.scaleMode),
    sdl_surface(other.sdl_surface),
    sdl_texture(other.sdl_texture)
{
    other.sdl_surface = nullptr;
    other.sdl_texture = nullptr;
}


void* Texture::Internal_GetSurface() const {
    return sdl_surface;
}
void* Texture::Internal_GetTexture() const {
    return sdl_texture;
}

TextureScaleMode Texture::GetScaleMode() const {
    return scaleMode;
}
void Texture::SetScaleMode(TextureScaleMode mode) {
    scaleMode = mode;

    SDL_SetTextureScaleMode(
        static_cast<SDL_Texture*>(sdl_texture),
        static_cast<SDL_ScaleMode>(static_cast<int>(mode))
    );
}

Color Texture::GetKeyColor() const {
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

    SDL_Renderer* renderer = static_cast<SDL_Renderer*>(PotatoEngine::GetWindowController()->RequestRenderingContext());

    sdl_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SetScaleMode(scaleMode);


}



Texture::~Texture() {
    SDL_DestroyTexture(static_cast<SDL_Texture*>(sdl_texture));
    SDL_DestroySurface(static_cast<SDL_Surface*>(sdl_surface));
}
