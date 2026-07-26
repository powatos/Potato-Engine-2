/** @file World.hpp */
#pragma once

#include <type_traits>
#include <concepts>
#include <vector>

#include "Game/Actor.hpp"

#include "Util/Vector2.hpp"

/** @brief Collection of actors in world */
using ActorPool = std::vector<Actor*>;

class Player;
class Block;

/**
 * @brief Local level managing gameplay interactions
 */
class World : public Tickable
{
public:

    /**
     * @brief Struct to hold world settings
     * @details This struct holds settings that affect world behavior like physics and collision settings
     */
    struct WorldSettings
    {
        Vector2 Size = Vector2(1000, 1000); ///< Size of world in game units
        float upGravity = 400.f; ///< Gravity strength when moving up for asymmetric actors
        float downGravity = 550.f; ///< Gravity strength when moving down for asymmetric actors
        bool doGravity = true; ///< Whether gravity should be applied to actors in world
        float clipDampeningFactor = 1.f; ///< Dampening factor for clipping collisions (1 = no dampening, 0 = full clip)
        float clipAllowed = 0.f; ///< Allowed penetration for clipping collisions in game units (0 = no penetration
        float bounceThreshold = 6.f; ///< Minimum normal velocity for bounce to occur in collisions
        bool enableDefaultWalls = false;
        float defaultWallThickness = 1;
    };

    /**
     * @brief Game settings for world
     * @sa WorldSettings
     */
    WorldSettings Settings;
    
    /**
     * @internal
     * @brief Constructs world
     */
    World();
    ~World();

    void BeginPlay();

    /**
     * @brief Spawns Actor into world
     * * @tparam ActorClass Class of actor to spawn
     * @returns Actor added to world
     */
    template<std::derived_from<Actor> ActorClass>
    ActorClass* SpawnActor();
    /**
     * @brief Spawns Actor into world
     * * @tparam ActorClass Class of actor to spawn
     * @param SpawnPosition Default world position for actor to be spawned at
     * @returns Actor added to world
     */
    template<std::derived_from<Actor> ActorClass>
    ActorClass* SpawnActor(const Vector2& SpawnPosition);

    /**
     * @brief Destroys actor from world
     * @param actor Actor to destroy
     */
    void DestroyActor(Actor* actor);
    /**
     * @brief Attempts to add external actor object to world managing system
     * @warning Intended for internal use only. Always prefer spawning using regular logic.
     * @sa SpawnActor
     * @returns actor if successful
     */
    Actor* AddtoPool(Actor* actor);

    /** @brief Gets count of actors in world @returns Count */
    inline size_t ActorCount() const { return actorPool.size(); }
    /** @brief Gets ActorPool @returns actor pool */
    const ActorPool& GetAllActors() const;

private:
    virtual void TickPostUpdate(float dt) override;

    void CreateDefaultWalls();
    void ResizeWalls();

    ActorPool actorPool;
    ActorPool destroyQueue;

    Block *wallB, *wallU, *wallL, *wallR;

};

template<std::derived_from<Actor> ActorClass>
ActorClass* World::SpawnActor() {
    static_assert(std::is_base_of_v<Actor, ActorClass>, "Illegal class spawn to world");

    ActorClass* actor = new ActorClass();
    AddtoPool(actor);

    // post-spawn functionality here
    actor->DispatchBeginPlay();

    return static_cast<ActorClass*>(actor);
}

template<std::derived_from<Actor> ActorClass>
ActorClass* World::SpawnActor(const Vector2& SpawnPosition) {
    Actor* actor = SpawnActor<ActorClass>();
    actor->SetPosition(SpawnPosition);

    return static_cast<ActorClass*>(actor);
}