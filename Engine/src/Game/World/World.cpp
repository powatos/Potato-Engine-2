/** @file World.cpp */

#include <algorithm>

#include "Actors/Block.hpp"
#include "Actors/Player.hpp"

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

    Block** walls[] = {&wallB, &wallL, &wallR, &wallU};

    for (auto& wall : walls) {
        *wall = SpawnActor<Block>();
        (*wall)->simpleColor = Color::BLACK();
        (*wall)->simpleColorFill = true;
    }

    // wallB = SpawnActor<Block>();
    // wallB->simpleColor = Color::BLACK();
    //
    // wallL = SpawnActor<Block>();
    // wallL->simpleColor = Color::BLACK();
    //
    // wallR = SpawnActor<Block>();
    // wallR->simpleColor = Color::BLACK();
    //
    // wallU = SpawnActor<Block>();
    // wallU->simpleColor = Color::BLACK();

    ResizeWalls();
}

void World::ResizeWalls() {
    /**
     * bottom and top extrude horizontally to fill corner
     *
     * -----
     * |   |
     * -----
     */
    const int wallWidth = Settings.defaultWallThickness;

    wallB->SetPosition(Vector2(-wallWidth, 0));
    wallB->SetSize(Vector2(Settings.Size.x + 2*wallWidth, wallWidth));

    wallL->SetPosition(Vector2(-wallWidth, Settings.Size.y));
    wallL->SetSize(Vector2(wallWidth, Settings.Size.y));

    wallR->SetPosition(Vector2(Settings.Size.x, Settings.Size.y));
    wallR->SetSize(Vector2(wallWidth, Settings.Size.y));

    wallU->SetPosition(Vector2(-wallWidth, Settings.Size.y + wallWidth));
    wallU->SetSize(Vector2(Settings.Size.x + 2*wallWidth, wallWidth));

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

Actor* World::AddToPool(Actor* actor) {
    if (actor == nullptr) { return nullptr; }

    for (auto it = actorPool.begin(); it != actorPool.end(); ++it) {
        if (actor->GetZIndex() < (*it)->GetZIndex()) {
            actorPool.insert(it, actor);
            return actor;
        }
    }

    actorPool.push_back(actor);
    return actor;
}

void World::ReorderActorByZIndex(const Actor* actor) {
    if (actor == nullptr) { return; }

    auto from = std::ranges::find(actorPool, actor);
    if (from == actorPool.end()) { return; }

    for (auto to = actorPool.begin(); to != actorPool.end(); ++to) {
        if (from == to) { continue; }

        if (actor->GetZIndex() < (*to)->GetZIndex()) {
            // moves *from to *to in-place

            if (to < from) {
                std::rotate(to, from, from + 1);
            } else {
                std::rotate(from, from+1, to + 1);
            }

            return;
        }
    }

}

World::~World() {

    while (!actorPool.empty()) {
        Actor* actor = actorPool.back();

        delete actor; // free memory
        actorPool.pop_back(); // erase nullptr
    }

}
