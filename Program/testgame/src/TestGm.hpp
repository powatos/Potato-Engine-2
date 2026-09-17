/** @file "TestGm.hpp" */
#pragma once
#include "Core/Gamemode.hpp"


struct ProgressBarElement;

class TestGM : public Gamemode
{
public:
    void BeginPlay() override;

    void updatePGB();
private:
    void computeWork();
    void work();
    void randomTerrain();

    ProgressBarElement* pgb;

};
