#pragma once

#include "Actors/Player.hpp"
#include "Game/Raycast.hpp"

class PlatformPlayer : public Player
{
public:
    PlatformPlayer();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float dt) override;
private:
    Raycast downraycast;
};