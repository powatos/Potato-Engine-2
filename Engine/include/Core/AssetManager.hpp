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
 * @details This class is intended for internal use, but may also be used to manipulate asset types directly.
 * @warning It is recommended to use the corresponding asset class manager to interact with assets (e.g. TextureManager for texture assets)
 */
class AssetManager : public EngineSubsystem<AssetManager>
{
    ENGINE_SUBSYSTEM(AssetManager)
public:
    AssetManager();
    ~AssetManager() = default;

    void Resolve() noexcept override;

    /**
     * @brief Gets an asset based on its corresponding file
     * @param relativePath Path from assets folder to the file associated with the asset
     * @returns Asset associated with file
     */
    Asset* GetAsset(const std::string& relativePath);

    /**
     * @brief Normalizes a path string corresponding to the target OS
     * @details On POSIX systems, all separators are replaced with '/'. On Windows systems, all separators are replaced with '\\'
     * @param path Directory path to normalize
     * @return
     */
    static std::string NormalizePath(const std::string& path);

    /**
     * @brief Gets the target assets directory from the OS home directory
     * @return Full path to assets directory
     */
    FilePath GetAssetsDir() const;

    /**
     * @brief Gets the target saves directory from the OS home directory
     * @return Full path to saves directory
     */
    FilePath GetSaveDir() const;

    /**
     * @brief Calls corresponding constructors to cache all assets under the assets folder
     * @note This function is intended for internal use only
     */
    void CacheAssets();

    /**
     * @brief Gets full asset cache
     * @note This function is intended for internal use only
     * @returns Current asset cache
     */
    const std::unordered_map<std::string, Asset*>& GetCache() const;

private:
    std::unordered_map<std::string, Asset*> cache;

};