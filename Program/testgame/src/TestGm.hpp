/** @file "TestGm.hpp" */
#pragma once
#include "Core/Gamemode.hpp"


class TestGM : public Gamemode
{
public:
    void BeginPlay() override;

private:
    void work();

};
