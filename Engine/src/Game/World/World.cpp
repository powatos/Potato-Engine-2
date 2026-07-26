/** @file World.cpp */

#include "Game/Block.hpp"
#include "Game/Player.hpp"

#include "Debug/Log.hpp"

#include "Game/World.hpp"

World::World() {

    Settings = WorldSettings();
    
}

void World::BeginPlay() {

    if (Settings.enableDefaultWalls) {
        CreateDefaultWalls();
    }
    
    for (Actor* actor : actorPool) {
        actor->DispatchBeginPlay();
    }
}

void World::DestroyActor(Actor* actor) {
    destroyQueue.push_back(actor);
}

void World::CreateDefaultWalls() {

    wallB = SpawnActor<Block>();
    wallB->simpleColor = Color::RED();

    wallL = SpawnActor<Block>();
    wallL->simpleColor = Color::RED();

    wallR = SpawnActor<Block>();
    wallR->simpleColor = Color::RED();

    wallU = SpawnActor<Block>();
    wallU->simpleColor = Color::RED();

    ResizeWalls();
}

void World::ResizeWalls() {
    const int wallWidth = Settings.defaultWallThickness;

    wallB->SetPosition(Vector2());
    wallB->SetSize(Vector2(Settings.Size.x, wallWidth));

    wallL->SetPosition(Vector2(-wallWidth, Settings.Size.y));
    wallL->SetSize(Vector2(wallWidth, Settings.Size.y));

    wallR->SetPosition(Vector2(Settings.Size.x, Settings.Size.y));
    wallR->SetSize(Vector2(wallWidth, Settings.Size.y));

    wallU->SetPosition(Vector2(0, Settings.Size.y + wallWidth));
    wallU->SetSize(Vector2(Settings.Size.x, wallWidth));

}

void World::TickPostUpdate(float dt) {

    for (auto it0 = destroyQueue.begin(); it0 != destroyQueue.end();) {
        auto it1 = std::find(actorPool.begin(), actorPool.end(), *it0);
    
        if (it1 != actorPool.end()) {
            delete *it1; // free memory
            actorPool.erase(it1); // erase nullptr
        }

        it0 = destroyQueue.erase(it0);

    }

    if (Settings.enableDefaultWalls) {
        if (wallB == nullptr) { CreateDefaultWalls(); }
        ResizeWalls();
    }

}

const ActorPool& World::GetAllActors() const
{
    return actorPool;
}

Actor* World::AddtoPool(Actor* actor) {
    if (actor == nullptr) { return nullptr; }

    actorPool.push_back(actor);
    return actor;
}

World::~World() {

    while (!actorPool.empty()) {
        Actor* actor = actorPool.back();

        delete actor; // free memory
        actorPool.pop_back(); // erase nullptr
    }

}
