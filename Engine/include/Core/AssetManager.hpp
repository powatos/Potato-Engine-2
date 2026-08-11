/** @file AssetManager.hpp */
#pragma once

#include <filesystem>
#include <functional>
#include <unordered_map>
#include <vector>
#include <array>
#include <typeindex>
#include <typeinfo>

#include "Asset.hpp"
#include "AssetConfigs.h"
#include "AssetManager.hpp"

#include "EngineSubsystem.hpp"
#include "Font.hpp"
#include "Texture.hpp"
#include "Debug/Log.hpp"

using FilePath = std::filesystem::path;
struct IAssetLoader;

struct AssetTypeDescriptor
{
    FilePath subdir;
    FilePath defaultAsset;
    std::vector<std::string_view> validExtensions;
    std::unique_ptr<IAssetLoader> loader;
};

struct IAssetLoader
{
    IAssetLoader() = default;
    virtual ~IAssetLoader() = default;
    [[nodiscard]] virtual Asset* LoadAsset(const FilePath& rootPath) const = 0;
};

struct TextureLoader : IAssetLoader
{
    [[nodiscard]] Asset* LoadAsset(const FilePath& rootPath) const override;
};

struct FontLoader : IAssetLoader
{
    [[nodiscard]] Asset* LoadAsset(const FilePath& rootPath) const override;
};

namespace Internal
{
    using AssetRegistryType = std::unordered_map<std::type_index, AssetTypeDescriptor>;

    inline AssetRegistryType& GetAssetRegistry() {
        static AssetRegistryType registry;
        return registry;
    }



}

static struct AssetLoaderRegistrar
{
    AssetLoaderRegistrar() {
        Internal::AssetRegistryType& registry = Internal::GetAssetRegistry();

        registry[std::type_index(typeid(Texture))] = {
            DEFAULT_TEXTURES_SUBDIR,
            DEFAULT_TEXTURE_ASSET_PATH,
            {".bmp"},
            std::make_unique<TextureLoader>(),
        };

        registry[std::type_index(typeid(Font))] = {
            DEFAULT_FONTS_SUBDIR,
            DEFAULT_FONT_ASSET_PATH,
            {".ttf"},
            std::make_unique<FontLoader>(),
        };

    }
} AssetLoaderRegistrar_i;

constexpr int DEFAULT_FONT_PT_SIZE = 8;

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
     * @brief Gets an asset based on its type
     * * @tparam AssetType type of the asset
     * @param relativePath Path from its subdirectory
     * @returns Asset associated with file
     */
    template<typename AssetType>
    AssetType* GetAsset(const std::string& relativePath) {
        const AssetTypeDescriptor& descriptor = Internal::GetAssetRegistry().at(std::type_index(typeid(AssetType)));

        const FilePath path = GetAssetsDir() / descriptor.subdir / NormalizePath(relativePath);

        auto returnDefaultAsset = [&]() -> AssetType* {
            return GetAsset<AssetType>((descriptor.defaultAsset).string());
        };

        if (!std::filesystem::exists(path)) {
            LOG(LogType::WARNING, "Attempt to get invalid asset: {}", path.string());
            return returnDefaultAsset();
        }

        if (const auto it = cache.find(path.string()); it != cache.end()) {
            return static_cast<AssetType*>(it->second);
        }

        return returnDefaultAsset();
    }

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
    [[nodiscard]] const std::unordered_map<std::string, Asset*>& GetCache() const;

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
    static FilePath GetAssetsDir();

    /**
     * @brief Gets the target saves directory from the OS home directory
     * @return Full path to saves directory
     */
    static FilePath GetSaveDir();

private:
    std::unordered_map<std::string, Asset*> cache;

};