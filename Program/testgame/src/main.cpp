
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

#include "Core/IScreenController.hpp"
#include "Util/TimerManager.hpp"

PROJECT("testgame")

int main()
{
    /// ENGINE SETUP
    PotatoEngine& engine = PotatoEngine::Get();
    engine.LoadSubclasses();
    GameInstance* instance = GameInstance::Get();

    IScreenController* screenController = engine.GetScreenController();
    screenController->SetFrameRate(60.f);
    screenController->SetBackgroundColor(Color(0x00, 0xaa, 0xff, 0xff));
    screenController->SetScreenSize(Vector2(500,500));

    /// LEVEL SETUP
    [[maybe_unused]] World* world = instance->GetWorld();
    PersistentLevel level("save1.json");
    level.LoadStaticActors();

    world->Settings.enableDefaultWalls = true;

    Block* Obstacle1 = world->SpawnActor<Block>(Vector2(10,50));
    Obstacle1->SetSize(Vector2(20, 20));
    Obstacle1->simpleColor = Color::BLUE();
    Obstacle1->SetCollisionType(CollisionType::Block);
    Obstacle1->SetSimulatingPhysics(true);
    Obstacle1->SetMovability(ActorMovability::Movable);
    Obstacle1->SetBounce(0.5f);

    /// PLAYER SETUP
    PlayerController* playerController = instance->GetPlayerController();
    Player* player = playerController->GetPlayer();
    player->SetPosition(Vector2(30, 10));
    player->GetTexture().SetRotation(0.f);
    player->SetUsingSimpleTexture(true);
    player->SetSize(Vector2(10,10));
    player->simpleColor = Color(0xff, 0xff, 0x00);
    player->simpleColorFill = true;

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
    engine.BeginPlay();

    engine.Resolve();

    return 0; 
}


