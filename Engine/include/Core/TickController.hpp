/** @file TickController.hpp */
#pragma once

#include <unordered_map>

#include "Core/EventDelegate.hpp"

#include "Core/EngineSubsystem.hpp"

enum class TickGroup;
struct __TickGroupHash;
class Tickable;

/**
 * 
 */
class TickController : public EngineSubsystem<TickController>
{
    ENGINE_SUBSYSTEM(TickController)

public:
    virtual void Resolve() noexcept override;

    /**
     * @brief Fires to all delegates in a group
     * @param dt DeltaTime in seconds
     * @param group group to fire to
     */
    void Fire(float dt, TickGroup group);

    /**
     * @brief Registers a Tickable object to be able to tick
     * @param tickable the tickable object to register
     */
    void Register(Tickable* tickable);
    /**
     * @brief Unregisters a Tickable object to be able to tick
     * @param tickable the tickable object to unregister
     */
    void Unregister(Tickable* tickable);    

protected:
    TickController();
    ~TickController();

private:
    std::vector<Tickable*> tickables;
    std::vector<Tickable*> tickablesQueue;

    constexpr void checkObjByGroup(float dt, TickGroup group, Tickable* obj);
};

/**
 * @enum TickGroup
 * @brief Represents a group for tick ordering 
 */
enum class TickGroup {
    PreInput, //< Stage before input is handled
    _Input, //< Stage where input is handled @warning For internal use only
    Update, //< Main game update stage
    PostUpdate, //< Stage after main game updates are proccessed
    _Physics, //< Stage where physics is handled @warning For internal use only
    PostPhysics, //< Stage after physics is proccessed
    _Render, //< Render stage
    PostRender //< Stage after rendering is complete
};
struct __TickGroupHash {
    size_t operator ()(const TickGroup& g) const noexcept{
        return static_cast<size_t>(g);
    }
};