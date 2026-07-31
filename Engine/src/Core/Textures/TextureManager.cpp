/** @file "TextureManager.cpp" */

#include "Core/TextureManager.hpp"

#include <ranges>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>

#include "Core/AssetManager.hpp"
#include "Core/IScreenController.hpp"
#include "Debug/Log.hpp"

#ifndef DEFAULT_TEXTURE_ASSET_PATH
    #error "Potato Engine: Must define DEFAULT_TEXTURE_ASSET_PATH before engine compilation"
#endif

TextureManager::TextureManager() {
    LOG(LogType::VITAL, "TextureManager constructed");

}

Texture* TextureManager::CreateTexture(const FilePath& rootPath) {

    SDL_Renderer* renderer = static_cast<SDL_Renderer*>(PotatoEngine::Get().GetScreenController()->RequestRenderingContext());
    if (renderer == nullptr) {
        LOG(LogType::ERROR, "Failed to get valid rendering context while creating texture with path {}", rootPath.string());
        return nullptr;
    }

    SDL_Surface* surface = SDL_LoadBMP(rootPath.string().c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    Texture* tex = new Texture( rootPath, surface, texture);
    tex->SetKeyColor(Color::BLANK());

    cache[rootPath.string()] = tex;

    return tex;
}



Texture* TextureManager::GetTexture(const std::string& relativePath) {
    const AssetManager* assetManager = AssetManager::Get();

    const std::string fullPath = AssetManager::NormalizePath((assetManager->Get()->GetAssetsDir() / relativePath).string());

    if (!cache.contains(fullPath)) {
        return GetTexture(DEFAULT_TEXTURE_ASSET_PATH);
    }

    return cache[fullPath];
}


void TextureManager::Resolve() noexcept {
    LOG(LogType::VITAL, "Resolving TextureManager");
}
