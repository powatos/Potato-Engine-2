/** @file PotatoEngine.cpp */

#include <cstdio>
#include <cstdlib>

#include "Core/GameInstance.hpp"
#include "Core/Gamemode.hpp"
#include "Core/EventController.hpp"
#include "Engine.hpp"
#include "Game/World.hpp"
#include "UIController.hpp"
#include "UI/HUDController.hpp"
#include "Core/TickController.hpp"
#include "PhysicsController.hpp"
#include "Core/InputController.hpp"
#include "Util/TimerManager.hpp"
#include "InputManager.hpp"
#include "OutputManager.hpp"

#include "Debug/Log.hpp"

#include <SDL3/SDL.h>

#include "Core/PotatoEngine.hpp"

#include "Core/AssetManager.hpp"
#include "Core/TextureManager.hpp"


PotatoEngine::PotatoEngine() {

    char* prefPath = SDL_GetPrefPath(___ENGINE_GLOBALS::orgName.c_str() ,___ENGINE_GLOBALS::appName.c_str());
    logPath = std::string(prefPath) + "debug.log";
    SDL_free(prefPath);

    LOG.init(logPath);
    LOG.showTerminalOutput = false;

    LOG(LogType::VITAL, "PotatoEngine constructed");

    SubsystemStack.push_back( AssetManager::Get() );
    SubsystemStack.push_back( TextureManager::Get() );

    SubsystemStack.push_back( Engine::Get() );

    SubsystemStack.push_back( TickController::Get() );

    SubsystemStack.push_back( OutputManager::Get() );
    SubsystemStack.push_back( InputManager::Get() );

    SubsystemStack.push_back( GameInstance::Get() );

    SubsystemStack.push_back( EventController::Get() );

    SubsystemStack.push_back( PhysicsController::Get() );

    SubsystemStack.push_back( UIController::Get() );
    SubsystemStack.push_back( TimerManager::Get() );

}

PotatoEngine& PotatoEngine::Get()
{
    static PotatoEngine engine;
    return engine;
}

void PotatoEngine::LoadSubclasses() {
    InputController = InputManager::Get();
    ScreenController = OutputManager::Get();
    HUDController = UIController::Get();
    NativeEventController = EventController::Get();

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

IScreenController* PotatoEngine::GetScreenController() const {
    return ScreenController;
}

IHUDController* PotatoEngine::GetHUDController() const {
    return HUDController;
}

EventController* PotatoEngine::GetNativeEventController() const {
    return NativeEventController;
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
