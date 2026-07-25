/** @file EngineSubsystem.hpp */
#pragma once

#include "PotatoEngine.hpp"
#include "Singleton.hpp"

/**
 * @brief Interface for internal engine classes
 * @note This class is intended for internal use
 */
class IEngineSubsystem
{
public:
    virtual ~IEngineSubsystem() = default;
    virtual void Resolve() noexcept {};
    virtual void BeginPlay() {};
};

/**
 * @brief Inheriting class for internal engine classes
 * @note This class is intended for internal use
 */
template<typename T>
class EngineSubsystem : public IEngineSubsystem, public Singleton<T> 
{};

/**
 * @def ENGINE_SUBSYSTEM(type)
 * @brief Adds singleton as a friend of the class to ensure proper Get retrieval
 * @details Call this on the first line of the class header
 * @note This macro is intended for internal use
 * @param type class name
 */
#define ENGINE_SUBSYSTEM(type) \
friend class Singleton<type>;

