/** @file Player.hpp */
#pragma once

#include "Game/Entity.hpp"

class PlayerController;

/**
 * @brief User controllable character
 */
class Player : public Entity
{

public:
    /**
     * @internal
     * @brief Constructs player
     */
    Player();
    virtual ~Player();

protected:
    virtual void BeginPlay() override;
    virtual void Tick([[maybe_unused]] float dt) override;

private:

};
