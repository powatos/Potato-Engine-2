/** @file "Font.cpp" */

#include "Core/Font.hpp"

#include <ranges>
#include <SDL3_ttf/SDL_ttf.h>



Font::Font(FilePath path, void* font) :
    Asset(path),
    font(font)
{
}

Font::Font(Font&& other) noexcept :
    Asset(std::move(other)),
    font(other.font)
{
    other.font = nullptr;
}

void* Font::___Get_Font() {
    return font;
}

Font::~Font() {
    TTF_CloseFont(static_cast<TTF_Font*>(font));
}
