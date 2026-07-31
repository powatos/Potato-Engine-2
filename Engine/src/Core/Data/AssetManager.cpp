/** @file AssetManager.cpp */

#include "Core/PotatoEngine.hpp"

#include "Debug/Log.hpp"

#include "SDL3/SDL.h"

#include "Core/AssetManager.hpp"

#include <ranges>

#include "Core/TextureManager.hpp"

AssetManager::AssetManager() {
    LOG(LogType::VITAL, "AssetManager constructed");
}

void AssetManager::CacheAssets() {
    for (const auto& entry : std::filesystem::recursive_directory_iterator(GetAssetsDir())) {
        if (!std::filesystem::is_regular_file(entry)) { continue; }

        const std::string ext = entry.path().extension().string();

        Asset* asset = nullptr;

        if (ext == ".bmp") {
            asset = TextureManager::Get()->CreateTexture(entry.path());
        }
        // else if (ext == ".png") {
        //
        // }
        // else if (ext == ".txt") {
        //
        // }

        if (asset == nullptr) {
            LOG(LogType::WARNING, "Invalid asset cached with path {}", entry.path().string());
        }

        cache[entry.path().string()] = asset;

    }
}

Asset* AssetManager::GetAsset(const std::string& relativePath) {
    const FilePath path = GetAssetsDir() / NormalizePath(relativePath);

    Asset* asset = cache[path.string()];

    if (asset == nullptr) {
        LOG(LogType::WARNING, "Attemping to get asset not yet cached: {}", relativePath);
    }

    return asset;

}

FilePath AssetManager::GetAssetsDir() const {
    const char* basePath = SDL_GetBasePath(); // same directory as executable
    FilePath assetsDir = FilePath(basePath) / "Assets";

    if (!std::filesystem::exists(assetsDir)) {
        LOG(LogType::ERROR, "Assets directory not found at {}", assetsDir.string());
    }

    return assetsDir;
}

FilePath AssetManager::GetSaveDir() const {
    char* prefPath = SDL_GetPrefPath(___ENGINE_GLOBALS::orgName.c_str() ,___ENGINE_GLOBALS::appName.c_str()); // local dir; win AppData\Roaming\orgName(PotatoEngine)\appName
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

const std::unordered_map<std::string, Asset*> & AssetManager::GetCache() const {
    return cache;
}

void AssetManager::Resolve() noexcept {
    LOG(LogType::VITAL, "Resolving AssetManager");

    for (auto& [name, asset] : cache) {
        delete asset;
        asset = nullptr;
    }

}
