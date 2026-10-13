#pragma once
#include "Core/Gamemode.hpp"

class PlatformerGamemode : public Gamemode
{
public:
    void BeginPlay() override;

private:
    Block* platform(const Vector2& pos, const Vector2& size);

};
