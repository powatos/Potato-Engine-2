/** @file GameInstance.cpp */

#include "Core/Gamemode.hpp"
#include "Actors/Player.hpp"
#include "Game/World.hpp"

#include "Debug/Log.hpp"

#include "Core/GameInstance.hpp"

GameInstance::GameInstance() {
    LOG(LogType::VITAL, "GameInstance constructed");
    
    Internal_IsMainTickRunning = false;

    world = new World();

}

void GameInstance::BeginPlay() {
    IEngineSubsystem::BeginPlay();

    // call BeginPlay on world (dispatches actors)
    world->BeginPlay();

    // call BeginPlay on subclasses
    // ActivePlayerController->BeginPlay();
    ActiveGamemode->BeginPlay();

    Internal_IsMainTickRunning = true;
}

void GameInstance::InstantiateSubclasses() {

    // User-defined Instantiaters
    auto c_pc = Internal::DefaultSubclassInstantiators::i_PlayerController();
    auto c_p = Internal::DefaultSubclassInstantiators::i_Player();
    auto c_gm = Internal::DefaultSubclassInstantiators::i_Gamemode();
    
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
    world->AddToPool(player);
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
    Internal_IsMainTickRunning = false;
}

void GameInstance::LoadSubclasses() {
    InstantiateSubclasses();

    ActivePlayerController->Initialize();
    ActivePlayerController->SetupInputBindings();
}

World* GameInstance::GetWorld() const { return world; }
PlayerController* GameInstance::GetPlayerController() const { return ActivePlayerController; }
Gamemode* GameInstance::GetGamemode() const { return ActiveGamemode; }

void GameInstance::Resolve() noexcept {
    LOG(LogType::VITAL, "Resolving GameInstance");

    delete ActivePlayerController;
    // ActivePlayer is freed as an actor on world destruction
    delete ActiveGamemode;

    delete world;
}

GameInstance::~GameInstance() {

}
