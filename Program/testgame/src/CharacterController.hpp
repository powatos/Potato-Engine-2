#pragma once

#include <Game/PlayerController.hpp>

class CharacterController : public PlayerController
{
public:
    CharacterController();
    
    void SetupInputBindings() override;

private:
    float moveSpeed;

    void moveup();
    void movedown();
    void moveright();
    void moveleft();

    void rayc();
};