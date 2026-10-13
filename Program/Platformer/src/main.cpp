
#include <Core/GameInstance.hpp>
#include <Core/PersistentLevel.hpp>
#include <Core/EventManager.hpp>
#include <Core/InputController.hpp>
#include <Core/PotatoEngine.hpp>
#include <Actors/Block.hpp>
#include <Actors/Player.hpp>
#include <Game/World.hpp>
#include <Widgets/DebugInfo.hpp>
#include <Core/Texture.hpp>

#include <Debug/Log.hpp>
#include <Util/Vector2.hpp>

#include <memory>

#include "Core/IWindowController.hpp"
#include "UI/TextElement.hpp"
#include "UI/UIManager.hpp"
#include "Util/GameplayUtil.hpp"
#include "Util/TimerManager.hpp"

PROJECT("Platformer")

int main()
{
    /// ENGINE SETUP
    PotatoEngine& engine = PotatoEngine::Get();
    engine.LoadSubclasses();

    GameInstance* instance = GameInstance::Get();

    IWindowController* windowController = PotatoEngine::GetWindowController();
    windowController->SetTargetFrameRate(60.f);
    windowController->SetBackgroundColor(Color(0x00, 0xaa, 0xff, 0xff));
    windowController->SetRescaleMode(WindowRescaleMode::Letterbox);
    windowController->SetWindowMode(WindowMode::Windowed);

    /// LEVEL SETUP
    [[maybe_unused]] World* world = instance->GetWorld();
    world->Settings.enableDefaultWalls = true;
    world->Settings.downGravity = 1500;
        world->Settings.upGravity = 1200;
    world->Settings.Size = Vector2(100,100);

    /// PLAYER SETUP
    PlayerController* playerController = instance->GetPlayerController();
    Player* player = playerController->GetPlayer();
    player->SetPosition(Vector2(0,10));

    /// UI SETUP
    UIManager* uim = UIManager::Get();

    /// DEBUG

    /// PLAY
    engine.BeginPlay();

    engine.Resolve();

    return 0; 
}


