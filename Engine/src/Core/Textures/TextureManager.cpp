/** @file TextureManager.cpp */

#include "Core/DataManager.hpp"

#include "Debug/Log.hpp"

#include "Core/TextureManager.hpp"

TextureManager::TextureManager() {
    LOG(LogType::VITAL, "TextureManager constructed");

    path texturesDir = DataManager::GetConfigDir() / "Textures";
    if (!std::filesystem::exists(texturesDir)) {
        LOG(LogType::ERROR, "Textures directory not found at {}", texturesDir.string());
        return;
    }

    for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(texturesDir)) {
        const path p = entry.path();

        if (entry.exists() && entry.is_regular_file() && p.extension() == ".txt") {
            RegisterTexture(p.filename().stem().string());
        } else {
            LOG(LogType::WARNING, "Invalid texture file found at {}", p.string());
        }
    }
}

void TextureManager::BeginPlay() {

}

void TextureManager::RegisterTexture(const std::string& textureFileName) {
    textures.emplace(textureFileName, textureFileName + ".txt");
}
const Texture& TextureManager::GetTexture(const std::string& name) {
    return textures.at(name);
}
bool TextureManager::HasTexture(const std::string& name) const {
    return textures.find(name) != textures.end();
}

void TextureManager::Resolve() noexcept {
    LOG(LogType::VITAL, "Resolving TextureManager");
}

TextureManager::~TextureManager() {
}