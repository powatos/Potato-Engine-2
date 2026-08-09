/** @file GameInstance.hpp */
#pragma once

#include <functional>
#include <type_traits>

#include "Game/World.hpp"
#include "Actors/Player.hpp"
#include "Gamemode.hpp"

#include "Core/EngineSubsystem.hpp"
#include "Core/Singleton.hpp"
#include "Game/PlayerController.hpp"

/**
 * @def SET_DEFAULT_SUBCLASS(def, set)
 * @relates GameInstance
 * @brief Registers classes as the default subclass for its class
 * @details Default subclasses are managed by the engine, but custom overrides can be made
 * by inheriting from the base.
 * @param def Base class to set default for
 * @param set User-defined class to set as default
 * @remark Call this macro at the top of the source file to set as default
 * @warning Trying to set multiple classes as the default for a subclass is undefined behaviour (see [static initialization
 * order fiasco](https://en.cppreference.com/cpp/language/siof))
 */
#define SET_DEFAULT_SUBCLASS(def, set) \
static struct ___##set##_DEFAULT_SUBCLASS_REGISTER { \
    ___##set##_DEFAULT_SUBCLASS_REGISTER() { \
        ___DEFAULT_INSTANTIATORS::_##def() = []() -> def* { return new set(); }; \
    } \
} ___##set##_DEFAULT_SUBCLASS_REGISTER_i;

/**
 * @brief Singleton with various game properties and functions. This class also manages game subclasses
 * @details Set global constants and get objects using the @ref get() "instance". \n Subclasses must be registered 
 * using @ref SET_DEFAULT_SUBCLASS(def, set) "SET_DEFAULT_SUBCLASS".
 */
class GameInstance : public EngineSubsystem<GameInstance>
{
    ENGINE_SUBSYSTEM(GameInstance)
    
public:
    virtual void Resolve() noexcept override;

    void LoadSubclasses();
    virtual void BeginPlay() override;

    /** @brief Gets @ref World "world" object @returns @ref World "World" object */
    World* GetWorld() const;
    /** @brief Gets @ref PlayerController "player controller" object @returns @ref PlayerController "player controller" object */
    PlayerController* GetPlayerController() const;
    /** @brief Gets @ref Gamemode "gamemode" object @returns Gamemode "gamemode" object */
    Gamemode* GetGamemode() const;

    /**
     * @brief Requests to queue engine shutdown
     * @details Engine will be shutdown at the end of the current tick when this function is called
     */
    void RequestShutdown();

    /**
     * @warning NEVER MODIFY THIS VALUE DIRECTLY
     * @sa RequestShutdown()
     */
    bool ___isMainTickRunning;

private:
    GameInstance();
    ~GameInstance();

private:

    PlayerController* ActivePlayerController;
    Gamemode* ActiveGamemode;

    World* world;

    void InstantiateSubclasses();

};

namespace ___DEFAULT_INSTANTIATORS {
    inline std::function<Player*()>& _Player() {
        static std::function<Player*()> f;
        return f;
    }
    inline std::function<PlayerController*()>& _PlayerController() {
        static std::function<PlayerController*()> f;
        return f;
    }
    inline std::function<Gamemode*()>& _Gamemode() {
        static std::function<Gamemode*()> f;
        return f;
    }
}