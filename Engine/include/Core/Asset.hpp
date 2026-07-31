/** @file Asset.hpp */
#pragma once

#include <filesystem>

using FilePath = std::filesystem::path;

class AssetManager;

/**
 * @brief Enum for asset types supported by the engine
 */
// enum class AssetType
// {
//     None,
//
//     bmp,
//     png,
//
//     txt,
//
// };


/**
 * @brief Lightweight struct to store metadata for an `Asset`
 */
struct AssetMetadata
{
    // AssetType type;

};

/**
 * @brief Wrapper struct to hold data related to a game asset
 * @details `Asset`s cannot be copied, moved or constructed manually. Associate a game asset file with a struct pointer with AssetManager::RetrieveAsset() @sa AssetManager
 */
struct Asset
{
private:
    friend class AssetManager;


protected:
    Asset(const FilePath& path);
    virtual ~Asset() = default;

public:

    const AssetMetadata meta;

    const FilePath path;

    Asset(const Asset& other) = delete; // remove copy construction
    Asset& operator=(const Asset& other) = delete; // remove copy assignment

    Asset(Asset&& other) noexcept = default; // default move constructor for efficient ownership transfer
    Asset& operator=(Asset&& other) = delete; // remove move assignment (existing assets cannot be removed from context)

};