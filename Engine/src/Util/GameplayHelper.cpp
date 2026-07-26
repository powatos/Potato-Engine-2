/** @file GameplayHelper.cpp */

#include <random>

#include "Core/GameInstance.hpp"
#include "Game/Actor.hpp"
#include "Game/Camera.hpp"
#include "Game/World.hpp"

#include "Util/Vector2.hpp"

#include "Util/GameplayHelper.hpp"

#include "Core/IScreenController.hpp"

namespace {
    static std::mt19937& randGen() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        return gen;
    }
}

Vector2 GameplayHelper::VecToScreenVec(const Vector2& worldPos) {

    return Vector2{
        worldPos.x,
        PotatoEngine::Get().GetScreenController()->GetScreenSize().y - worldPos.y
    };

}

std::vector<Actor*> GameplayHelper::GetOverlappingActors(const Actor* actor) {
    std::vector<Actor*> result{};

    for (Actor* otherActor : GameInstance::Get()->GetWorld()->GetAllActors()) {
        if (actor != otherActor && IsActorOverlapping(actor, otherActor)) {
            result.push_back(otherActor);
        }
    }

    return result;
}

bool GameplayHelper::IsActorOverlapping(const Actor* actor1, const Actor* actor2) {
    if (actor1 == actor2) { return true; }

    const Vector2 a1Pos = actor1->GetPosition();
    const Vector2 a1Siz = actor1->GetSize();
    const Vector2 a2Pos = actor2->GetPosition();
    const Vector2 a2Siz = actor2->GetSize();

    return 
        a1Pos.x < a2Pos.x + a2Siz.x &&  // a1 left < a2 right
        a1Pos.x + a1Siz.x > a2Pos.x &&  // a1 right > a2 left
        a1Pos.y > a2Pos.y - a2Siz.y &&  // a1 top > a2 bottom
        a1Pos.y - a1Siz.y < a2Pos.y     // a1 bottom < a2 top
    ;
}

int GameplayHelper::RandomInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(randGen());
}

int GameplayHelper::RandomFloat(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(randGen());
}

int GameplayHelper::RandomBool() {
    return std::bernoulli_distribution(0.5)(randGen());
}