/** @file "FontManager.cpp" */

#include "Core/FontManager.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include "Core/AssetConfigs.h"
#include "Core/AssetManager.hpp"
#include "Debug/Log.hpp"

#ifndef DEFAULT_FONT_ASSET_PATH
    #error "Potato Engine: Must define DEFAULT_FONT_ASSET_PATH before engine compilation"
#endif

#ifndef DEFAULT_FONTS_SUBDIR
    #error "Potato Engine: Must define DEFAULT_FONTS_SUBDIR before engine compilation"
#endif

FontManager::FontManager() {
    LOG(LogType::VITAL, "FontManager constructed");
}
Font* FontManager::CreateFont(const FilePath& rootPath, const int ptSize) {
    TTF_Font* ttf_font = TTF_OpenFont(rootPath.string().c_str(), ptSize);

    Font* font = new Font(rootPath, ttf_font);

    cache[rootPath.string()][ptSize] = font;
    return font;
}

Font* FontManager::GetFont(const std::string& relativePath, const int ptSize) {
    const AssetManager* assetManager = AssetManager::Get();

    const std::string rootPath = AssetManager::NormalizePath((assetManager->GetAssetsDir() / DEFAULT_FONTS_SUBDIR / relativePath).string());

    if (!cache.contains(rootPath)) {
        return GetFont(DEFAULT_FONT_ASSET_PATH, ptSize);
    }

    if (!cache[rootPath].contains(ptSize)) {
        return CreateFont(rootPath, ptSize);
    }

    return cache[rootPath][ptSize];
}

Font* FontManager::ChangeFontSize(const Font* font, const int ptSize) {

    const std::string rootPath = font->path.string();

    if (!cache.contains(rootPath)) {
        LOG(LogType::ERROR, "ChangeFontSize called with unknown font: {}", rootPath);
        return nullptr;
    }

    if (!cache[rootPath].contains(ptSize)) {
        return CreateFont(rootPath, ptSize);
    }

    return cache[rootPath][ptSize];
}

void FontManager::Resolve() noexcept {
    LOG(LogType::VITAL, "Resolving FontManager");
}