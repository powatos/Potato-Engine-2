/** @file AssetManager.cpp */

#include <ranges>

#include "Core/PotatoEngine.hpp"
#include "Core/AssetManager.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include "Core/FontManager.hpp"
#include "Core/IWindowController.hpp"

#include "SDL3/SDL.h"

AssetManager::AssetManager() {
    LOG(LogType::VITAL, "AssetManager constructed");

    CacheAssets();
}

Asset* TextureLoader::LoadAsset(const FilePath& rootPath) const {
    Texture* tex = new Texture(rootPath);
    return tex;
}

Asset* FontLoader::LoadAsset(const FilePath& rootPath) const {
    Font* font = new Font(rootPath, DEFAULT_FONT_PT_SIZE);
    return font;
}

Asset* PresetLoader::LoadAsset(const FilePath& rootPath) const {
    Preset* preset = new Preset(rootPath);
    return preset;
}


void AssetManager::CacheAssets() {

    for (auto& [subdir, _, validExtensions, loader]: Internal::GetAssetRegistry() | std::views::values) {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(GetAssetsDir() / subdir)) {

            if (!std::filesystem::is_regular_file(entry)) { continue; }

            const std::string ext = entry.path().extension().string();
            if (std::ranges::find(validExtensions, ext) == validExtensions.end()) {
                LOG(LogType::WARNING, "Invalid asset extension found: {}", entry.path().string());
            }

            Asset* asset = loader->LoadAsset(entry.path());
            cache[entry.path().string()] = {
                asset
            };
        }
    }

}

FilePath AssetManager::GetAssetsDir() {
    const char* basePath = SDL_GetBasePath(); // same directory as executable
    FilePath assetsDir = FilePath(basePath) / "Assets";

    if (!std::filesystem::exists(assetsDir)) {
        LOG(LogType::ERROR, "Assets directory not found at {}", assetsDir.string());
    }

    return assetsDir;
}

FilePath AssetManager::GetSaveDir() {
    char* prefPath = SDL_GetPrefPath(Internal::EngineGlobals::orgName.c_str() ,Internal::EngineGlobals::appName.c_str()); // local dir; win AppData\Roaming\orgName(PotatoEngine)\appName
    FilePath saveDir = FilePath(prefPath) / "Saves";
    SDL_free(prefPath);

    if (!std::filesystem::exists(saveDir)) {
        LOG(LogType::ERROR, "Saves directory not found at {}", saveDir.string());
    }

    return saveDir;
}

std::string AssetManager::NormalizePath(const std::string& p_path) {
    std::string result(p_path);

    for (char& c : result) {
        if (c == '/' || c == '\\') {
            c = std::filesystem::path::preferred_separator;
        }
    }

    return result;
}

const std::unordered_map<std::string, Asset*>& AssetManager::GetCache() const {
    return cache;
}

void AssetManager::Resolve() noexcept {
    LOG(LogType::VITAL, "Resolving AssetManager");

    for (auto& asset: cache | std::views::values) {
        delete asset;
        asset = nullptr;
    }

}
