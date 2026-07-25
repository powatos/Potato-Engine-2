#pragma once

#include "Game/Player.hpp"
#include "Game/Raycast.hpp"

class MyPlayer : public Player
{
public:
    MyPlayer();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float dt) override;
private:
    Raycast downraycast;
};