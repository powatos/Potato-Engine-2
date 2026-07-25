/** @file DataManager.hpp */
#pragma once

#include <filesystem>

using path = std::filesystem::path;

/**
 * @brief A helper class to manage data and save directories
 * @details This class provides static functions to get data and save directories
 * and ensures they exist for each operating system.
 * @note This class is intended for internal use only
 */
class DataManager
{
public:
    DataManager() = default;
    ~DataManager() = default;

    static path GetConfigDir();
    static path GetSaveDir();

};