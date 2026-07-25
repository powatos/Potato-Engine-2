
#include <Core/GameInstance.hpp>
#include <Core/PersistentLevel.hpp>
#include <Core/EventController.hpp>
#include <Core/InputController.hpp>
#include <Core/PotatoEngine.hpp>
#include <Game/Block.hpp>
#include <Game/Player.hpp>
#include <Game/World.hpp>
#include <UI/HUDController.hpp>
#include <UI/DebugInfo.hpp>
#include <Core/TextureManager.hpp>
#include <Core/Texture.hpp>

#include <Debug/Log.hpp>
#include <Util/Vector2.hpp>

#include <memory>

#include "Util/TimerManager.hpp"

PROJECT("testgame")

int main()
{
    /// ENGINE SETUP
    PotatoEngine& engine = PotatoEngine::Get();
    engine.LoadSubclasses();
    GameInstance* instance = GameInstance::Get();
    
    // ideal 195
    instance->MS_REPEAT_THRESHOLD = 195;
    instance->FRAMES_PER_SECOND = 60.f;

    /// LEVEL SETUP
    [[maybe_unused]] World* world = instance->GetWorld();
    PersistentLevel level("save1.json");
    level.LoadStaticActors();

    world->Settings.setupDefaultWalls = true;
    // world->Settings.doGravity = false;
    // world->Settings.Size = Vector2(80,24);

    Block* Obstacle1 = world->SpawnActor<Block>(Vector2(10,2));
    Obstacle1->SetSize(Vector2(5, 3));
    Obstacle1->ctex = 'O';
    Obstacle1->SetCollisionType(CollisionType::Block);
    Obstacle1->SetSimulatingPhysics(true);
    Obstacle1->SetMovability(ActorMovability::Movable);
    Obstacle1->SetBounce(0.5f);

    /// PLAYER SETUP
    PlayerController* playerController = instance->GetPlayerController();
    Player* player = playerController->GetPlayer();
    player->SetPosition(Vector2(10, 10));
    player->GetTexture().SetRotation(0.f);
    player->SetUsingCTex(true);
    player->SetSize(Vector2(1,1));
    player->ctex = 'P';

    /// UI SETUP
    [[maybe_unused]] IHUDController* HUDController = engine.GetHUDController();

    /// DEBUG
    [[maybe_unused]] DebugInfo* debugInfoWidget = HUDController->AddWidget<DebugInfo>("W_DebugInfo");
    engine.GetInputController()->RegisterInputBinding(InputBinding(
        Keycode::T, 
        InputType::Impulse, 
        "ToggleDebugInfo", 
        static_cast<UIElement*>(debugInfoWidget), 
        &DebugInfo::ToggleVisibility
    ));

    /// PLAY
    engine.BeginPlay(true);

    engine.Resolve();

    return 0; 
}


