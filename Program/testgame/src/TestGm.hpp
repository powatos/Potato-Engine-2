/** @file "TestGm.hpp" */
#pragma once
#include "Core/Gamemode.hpp"


struct ProgressBarElement;

class PlatformerGamemode : public Gamemode
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
