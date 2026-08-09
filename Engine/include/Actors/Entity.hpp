/** @file Entity.hpp */
#pragma once

#include "Game/Actor.hpp"

/**
 * @brief Character with player-like attributes
 */
class Entity : public Actor 
{
public:
    /**
     * @internal
     * @brief Constructs entity
     */
    Entity();
    virtual ~Entity();

    /**
     * @brief Delivers damage to the entity
     * @param damage Damage to deliver
     * @returns Health after damage (>= 0)
     */
    float TakeDamage(float damage);

    /**
     * @brief Adds health to entity
     * @param amount Amount of health to add
     * @returns Health after addition
     */
    float AddHealth(float amount);
    
    /** @brief Gets health @returns Health of entity */
    float GetHealth() const;

protected:
    float Health;
    float MaxHealth;

};