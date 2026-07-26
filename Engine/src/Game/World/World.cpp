/** @file World.cpp */

#include "Game/Block.hpp"
#include "Game/Player.hpp"

#include "Debug/Log.hpp"

#include "Game/World.hpp"

World::World() {

    Settings = WorldSettings();
    
}

void World::BeginPlay() {

    if (Settings.setupDefaultWalls) {
        const int wallWidth = Settings.defaultWallSize;

        Block* wallB = SpawnActor<Block>(Vector2(-wallWidth, -1));
        wallB->SetSize(Vector2(Settings.Size.x + 2*wallWidth, wallWidth));
        wallB->simpleColor = Color::RED();

        Block* wallL = SpawnActor<Block>(Vector2(-wallWidth, Settings.Size.y + wallWidth));
        wallL->SetSize(Vector2(wallWidth, Settings.Size.y + 2*wallWidth));
        wallL->simpleColor = Color::RED();

        Block* wallR = SpawnActor<Block>(Vector2(Settings.Size.x, Settings.Size.y + wallWidth));
        wallR->SetSize(Vector2(wallWidth, Settings.Size.y + 2*wallWidth));
        wallR->simpleColor = Color::RED();

        Block* wallU = SpawnActor<Block>(Vector2(-wallWidth, Settings.Size.y + wallWidth-1));
        wallU->SetSize(Vector2(Settings.Size.x + wallWidth*2, wallWidth));
        wallU->simpleColor = Color::RED();
    }
    
    for (Actor* actor : actorPool) {
        actor->DispatchBeginPlay();
    }
}

void World::DestroyActor(Actor* actor) {
    destroyQueue.push_back(actor);
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
