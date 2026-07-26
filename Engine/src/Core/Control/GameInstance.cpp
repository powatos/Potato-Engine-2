/** @file GameInstance.cpp */

#include "Core/Gamemode.hpp"
#include "Game/Player.hpp"
#include "Game/World.hpp"

#include "Debug/Log.hpp"

#include "Core/GameInstance.hpp"

GameInstance::GameInstance() {
    LOG(LogType::VITAL, "GameInstance constructed");
    
    ___isMainTickRunning = false;

    world = new World();

    InstantiateSubclasses();
}

void GameInstance::BeginPlay() {
    IEngineSubsystem::BeginPlay();

    // call BeginPlay on world (dispatches actors)
    world->BeginPlay();

    // call BeginPlay on subclasses
    // ActivePlayerController->BeginPlay();
    ActiveGamemode->BeginPlay();

    ___isMainTickRunning = true;
}

void GameInstance::InstantiateSubclasses() {

    // User-defined Instantiaters
    auto c_pc = ___DEFAULT_INSTANTIATORS::_PlayerController();
    auto c_p = ___DEFAULT_INSTANTIATORS::_Player();
    auto c_gm = ___DEFAULT_INSTANTIATORS::_Gamemode();
    
    // PlayerController
    if (c_pc == nullptr) {
        LOG(LogType::WARNING, "No PlayerController default instantiator on GameInstance. Fallback to engine default.");
        ActivePlayerController = new PlayerController();
    } else { 
        ActivePlayerController = c_pc();
    }

    // Player
    Player* player;
    if (c_p == nullptr) {
        LOG(LogType::WARNING, "No Player default instantiator on GameInstance. Fallback to engine default.");
        player = new Player();
    } else {
        player = c_p();
    }
    world->AddtoPool(player);
    ActivePlayerController->AssignPlayer(player);

    // Gamemode
    if (c_gm == nullptr) {
        LOG(LogType::WARNING, "No Gamemode default instantiator on GameInstance. Fallback to engine default.");
        ActiveGamemode = new Gamemode();
    } else {
        ActiveGamemode = c_gm();
    }

}

void GameInstance::RequestShutdown() {
    ___isMainTickRunning = false;
}

void GameInstance::LoadSubclasses() {
    ActivePlayerController->Initialize();
    ActivePlayerController->SetupInputBindings();
}

World* GameInstance::GetWorld() const { return world; }
PlayerController* GameInstance::GetPlayerController() const { return ActivePlayerController; }
Gamemode* GameInstance::GetGamemode() const { return ActiveGamemode; }

void GameInstance::Resolve() noexcept {
    LOG(LogType::VITAL, "Resolving GameInstance");

    delete ActivePlayerController;
    delete world;
}

GameInstance::~GameInstance() {

}
