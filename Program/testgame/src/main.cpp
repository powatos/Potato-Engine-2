
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
#include "Core/TextureManager.hpp"
#include "UI/TextElement.hpp"
#include "UI/UIManager.hpp"
#include "Util/GameplayUtil.hpp"
#include "Util/TimerManager.hpp"

PROJECT("testgame")

int main()
{
    /// ENGINE SETUP
    PotatoEngine& engine = PotatoEngine::Get();
    engine.LoadSubclasses();

    GameInstance* instance = GameInstance::Get();

    IWindowController* windowController = engine.GetWindowController();
    windowController->SetTargetFrameRate(60.f);
    windowController->SetBackgroundColor(Color(0x00, 0xaa, 0xff, 0xff));
    // screenController->SetScreenResolution(Vector2(100,100));
    windowController->SetRescaleMode(WindowRescaleMode::Letterbox);
    windowController->SetWindowMode(WindowMode::Windowed);

    /// LEVEL SETUP
    [[maybe_unused]] World* world = instance->GetWorld();
    PersistentLevel level("save1.json");
    level.LoadStaticActors();

    world->Settings.enableDefaultWalls = true;

    /// PLAYER SETUP
    PlayerController* playerController = instance->GetPlayerController();
    Player* player = playerController->GetPlayer();
    player->SetPosition(Vector2(0,100));

    /// UI SETUP
    UIManager* uim = UIManager::Get();

    /// DEBUG
    DebugInfo* debugInfoWidget = uim->AddUI<DebugInfo>("W_DebugInfo");
    engine.GetInputController()->RegisterInputBinding(InputBinding(
        Keycode::T,
        InputType::Impulse,
        "ToggleDebugInfo",
        debugInfoWidget,
        &DebugInfo::ToggleVisibility
    ));

    /// PLAY
    engine.BeginPlay();

    engine.Resolve();

    return 0; 
}


