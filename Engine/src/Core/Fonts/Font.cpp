/** @file "Font.cpp" */

#include "Core/Font.hpp"

#include <ranges>
#include <SDL3_ttf/SDL_ttf.h>

Font::Font(const FilePath& path, int size) :
    Asset(path)
{
    TTF_Font* font = TTF_OpenFont(path.string().c_str(), static_cast<float>(size));
    ttf_fonts[size] = font;

    cachedDefaultSize = size;
}

Font::Font(Font&& other) noexcept :
    Asset(std::move(other)),
    ttf_fonts(std::move(other.ttf_fonts))
{
    for (auto& font : other.ttf_fonts | std::views::values) {
        font = nullptr;
    }
}

void* Font::Internal_GetFont(int size) {
    if (size == -1) { return ttf_fonts[cachedDefaultSize]; }

    if (auto it = ttf_fonts.find(size); it != ttf_fonts.end()) {
        return it->second;
    }

    TTF_Font* font = TTF_OpenFont(path.string().c_str(), static_cast<float>(size));
    ttf_fonts[size] = font;
    return font;
}

Font::~Font() {
    for (const auto& font : ttf_fonts | std::views::values) {
        TTF_CloseFont(static_cast<TTF_Font*>(font));
    }

}
