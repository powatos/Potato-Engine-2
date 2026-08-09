/** @file PotatoEngine.cpp */

#include <cstdio>
#include <cstdlib>

#include "Core/GameInstance.hpp"
#include "Core/Gamemode.hpp"
#include "Core/EventManager.hpp"
#include "Engine.hpp"
#include "Game/World.hpp"
#include "UI/UIManager.hpp"
#include "Core/TickManager.hpp"
#include "PhysicsManager.hpp"
#include "Core/InputController.hpp"
#include "Util/TimerManager.hpp"
#include "InputManager.hpp"
#include "OutputManager.hpp"

#include "Debug/Log.hpp"

#include <SDL3/SDL.h>

#include "Core/PotatoEngine.hpp"

#include "Core/AssetManager.hpp"
#include "Core/FontManager.hpp"
#include "Core/TextureManager.hpp"


PotatoEngine::PotatoEngine() {
    LOG(LogType::VITAL, "PotatoEngine constructed");

    /// Set up logs
    char* prefPath = SDL_GetPrefPath(___ENGINE_GLOBALS::orgName.c_str() ,___ENGINE_GLOBALS::appName.c_str());
    logPath = std::string(prefPath) + "debug.log";
    SDL_free(prefPath);

    LOG.init(logPath);
    // LOG.showTerminalOutput = false;

    /// Initialize all specific asset managers
    SubsystemStack.push_back( TextureManager::Get() );
    SubsystemStack.push_back( FontManager::Get() );

    /// Initialize low level controllers
    SubsystemStack.push_back( Engine::Get() );
    SubsystemStack.push_back( TickManager::Get() );

    /// Initialize IO controllers
    SubsystemStack.push_back( OutputManager::Get() );
    SubsystemStack.push_back( InputManager::Get() );

    /// Initialize asset manager
    SubsystemStack.push_back( AssetManager::Get() );

    /// Initialize game core classes
    SubsystemStack.push_back( GameInstance::Get() );

    /// Initialize event controller
    SubsystemStack.push_back( EventManager::Get() );

    /// Initialize physics controllers
    SubsystemStack.push_back( PhysicsManager::Get() );

    /// Initialize high level controllers
    SubsystemStack.push_back( UIManager::Get() );
    SubsystemStack.push_back( TimerManager::Get() );

}

PotatoEngine& PotatoEngine::Get()
{
    static PotatoEngine engine;
    return engine;
}

void PotatoEngine::LoadSubclasses() {
    InputController = InputManager::Get();
    WindowController = OutputManager::Get();

    AssetManager::Get()->CacheAssets();

    GameInstance::Get()->LoadSubclasses();
}

void PotatoEngine::BeginPlay(bool autoResolve)
{
    Engine* engine = Engine::Get();

    // Bind exit key (intentional lambda to surpress unregisters)
    InputController->RegisterInputBinding(InputBinding(
        Keycode::Escape, 
        InputType::Impulse, 
        "ExitGame", 
        []() { GameInstance::Get()->RequestShutdown(); }
    ));

    // Dispatch on subsystems
    for (IEngineSubsystem* sys : SubsystemStack) {
        sys->BeginPlay();
    }

    if (autoResolve) {
        TimerManager::Get()->AddTimer(
           "AutoResolve",
           5.0,
           GameInstance::Get(),
           &GameInstance::RequestShutdown
        );
    }

    // Begin game loop
    engine->main();
}

IInputController* PotatoEngine::GetInputController() const {
    return InputController;
}

IWindowController* PotatoEngine::GetWindowController() const {
    return WindowController;
}

void PotatoEngine::Resolve() noexcept {
    LOG(LogType::VITAL, "Resolving PotatoEngine (Subsystem stack resolve)");

    while (!SubsystemStack.empty()) {
        SubsystemStack.back()->Resolve();
        SubsystemStack.pop_back();
    }

    LOG(LogType::VITAL, "Subsystem stack resolved");
    LOG(LogType::VITAL, "Detailed log file can be found at {}", logPath.c_str());
}


PotatoEngine::~PotatoEngine() {
}
