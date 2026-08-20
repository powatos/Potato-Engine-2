/** @file Asset.hpp */
#pragma once

#include <filesystem>

using FilePath = std::filesystem::path;

/**
 * @brief Lightweight struct to store metadata for an `Asset`
 */
struct AssetMetadata
{

};

/**
 * @brief Wrapper struct to hold data related to a game asset
 * @details `Asset`s cannot be copied, move constructed, or constructed manually. Use the specific asset class type to handle game assets
 */
struct Asset
{
    /**
     * @brief Construct asset
     * @warning For internal use only!
     */
    Asset(const FilePath& path);

public:
    virtual ~Asset() = default;

    const AssetMetadata meta; ///< @brief Metadata for asset
    const FilePath path; ///< @brief Path to file associated with asset; intended for internal use

    Asset(const Asset& other) = delete; // remove copy construction
    Asset& operator=(const Asset& other) = delete; // remove copy assignment

    Asset(Asset&& other) noexcept = default; // default move constructor for efficient ownership transfer
    Asset& operator=(Asset&& other) = delete; // remove move assignment (existing assets cannot be removed from context)

};