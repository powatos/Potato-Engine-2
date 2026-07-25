/** @file Gamemode.hpp */
#pragma once

/**
 * @brief Base class for the gamemode, managing in-game logic and rules
 * @details Override BeginPlay() to setup custom game logic 
 */
class Gamemode
{
public:
    Gamemode() = default;
    virtual ~Gamemode() = default;

    /**
     * @brief Called automatically at the start of the game
     * @details Override to setup gameplay logic
     * @note Always call base method when overriding:
     * @code
     * void CustomGamemode::BeginPlay() {
     *     BaseGamemode::BeginPlay();
     *     // ...
     * }
     * @endcode
     */
    virtual void BeginPlay();

    
};