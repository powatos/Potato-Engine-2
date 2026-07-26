#pragma once

#include <Game/PlayerController.hpp>

class CharacterController : public PlayerController
{
public:
    CharacterController();
    
    void SetupInputBindings() override;

private:
    void zoomOut();
    void zoomIn();

};