/** @file DataManager.hpp */
#pragma once

#include <filesystem>
#include <functional>
#include <unordered_map>
#include <vector>

#include "Asset.hpp"

#include "EngineSubsystem.hpp"

using FilePath = std::filesystem::path;

/**
 * @brief Manager for asset file paths and directories
 * @details This class provides an interface to use assets
 */
class AssetManager : public EngineSubsystem<AssetManager>
{
    ENGINE_SUBSYSTEM(AssetManager)
public:
    AssetManager();
    ~AssetManager() = default;

    void Resolve() noexcept override;

    Asset* GetAsset(const std::string& relativePath);

    static std::string NormalizePath(const std::string& path);

    FilePath GetAssetsDir() const;
    FilePath GetSaveDir() const;

    void CacheAssets();
    const std::unordered_map<std::string, Asset*>& GetCache() const;

private:
    std::unordered_map<std::string, Asset*> cache;

};