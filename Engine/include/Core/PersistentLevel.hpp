/** @file PersistentLevel.hpp */
#pragma once

#include <fstream>
#include <string>

#include "DataManager.hpp"

#include "Util/Vector2.hpp"

/** 
 * @brief Level object that stores persistent save data
 * @remark Persistence is currently still under development
 * @details This object serves as a wrapper around a save state file and manages read/write
 * operations, as well as persistent game object loading. Note that the level manages all save data,
 * not only world data.
*/
struct PersistentLevel
{
    /**
     * @brief Construct level object
     * @param saveFileName Name of save file
     */
    PersistentLevel(const std::string& saveFileName);
    ~PersistentLevel() = default;

    /**
     * @brief Loads all static actors into the world
     * @returns true if all static actors were loaded successfully
     */
    bool LoadStaticActors();

    /**
     * @brief Get save data from a key
     * * @tparam Type Type of data to get
     * @param key Key to get data from
     * @returns Data
     */
    template <typename Type>
    Type GetData(std::string key) const;

    /**
     * @brief Write save data to a key
     * * @tparam Type Type of data to write
     * @param key Key to write data to
     * @param value Data
     */
    template <typename Type>
    void WriteData(std::string key, Type value);

    /**
     * @brief Default fallback save file format
     * @details This string is used as a placeholder whent the save file is 
     * not found or empty
     */
    inline static const std::string fallbackSaveString = R"(
    {
        "StaticActors" : {
            
        },
        "DynamicActors" : {

        },
        "Data" : {
            
        }
    }   
    )";

private:
    int GetIntData( std::string key ) const;
    float GetFloatData( std::string key ) const;
    std::string GetStringData( std::string key ) const;
    Vector2 GetVector2Data( std::string key ) const;

    void WriteIntData( std::string key, int value );
    void WriteFloatData( std::string key, float value );
    void WriteStringData( std::string key, std::string value );
    void WriteVector2Data( std::string key, const Vector2& value );

    path saveFileAbsPath;

};


template <typename Type>
Type PersistentLevel::GetData(std::string key) const {
    if      constexpr (std::is_same_v<Type, int>) {
        return GetIntData(key);
    }
    else if constexpr (std::is_same_v<Type, float>) {
        return GetFloatData(key);
    }
    else if constexpr (std::is_same_v<Type, std::string>) {
        return GetStringData(key);
    }
    else if constexpr (std::is_same_v<Type, Vector2>) {
        return GetVector2Data(key);
    }
}

template <typename Type>
void PersistentLevel::WriteData(std::string key, Type value) {
    if      constexpr (std::is_same_v<Type, int>) {
        WriteIntData(key, value);
    }
    else if constexpr (std::is_same_v<Type, float>) {
        WriteFloatData(key, value);
    }
    else if constexpr (std::is_same_v<Type, std::string>) {
        WriteStringData(key, value);
    }
    else if constexpr (std::is_same_v<Type, Vector2>) {
        WriteVector2Data(key, value);
    }
}