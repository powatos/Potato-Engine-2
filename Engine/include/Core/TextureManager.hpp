/** @file "TextureManager.hpp" */
#pragma once

#include <unordered_map>

#include "EngineSubsystem.hpp"
#include "Texture.hpp"

#include "AssetConfigs.h"

class TextureManager : public EngineSubsystem<TextureManager>
{
    ENGINE_SUBSYSTEM(TextureManager)

public:
    TextureManager();
    void Resolve() noexcept override;

    Texture* GetTexture(const std::string& relativePath);
    Texture* CreateTexture(const FilePath& rootPath);
private:

    std::unordered_map<std::string, Texture*> cache;
};

