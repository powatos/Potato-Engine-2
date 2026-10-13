/** @file "TestGm.cpp" */


#include "Core/GameInstance.hpp"
#include "Actors/Block.hpp"

#include "TestGm.hpp"

#include "Core/InputController.hpp"
#include "UI/UIManager.hpp"
#include "UI/ProgressBarElement.hpp"
#include "Util/GameplayUtil.hpp"

SET_DEFAULT_SUBCLASS(Gamemode, PlatformerGamemode)

void PlatformerGamemode::BeginPlay() {
    Gamemode::BeginPlay();

    // work();
    // computeWork();
    randomTerrain();

    UIManager* uim = UIManager::Get();
    pgb = uim->AddUI<ProgressBarElement>("playBar", uim->RootWidget);

    pgb->SetScreenPosition({0.5, 0, 0, 50});
    pgb->SetScreenSize({0, 0, 100, 10});

    pgb->maxValue = 100.f;

    PotatoEngine::GetInputController()->RegisterInputBinding(InputBinding(
        Keycode::F,
        InputType::Impulse,
        "updateBar",
        this,
        &PlatformerGamemode::updatePGB
    ));

}

void PlatformerGamemode::updatePGB() {
    LOG(LogType::DEBUG, "progress updated");
    pgb->AddValue(1.f);
}

void PlatformerGamemode::randomTerrain() {

    World* world = GameInstance::Get()->GetWorld();

    const float maxLength = world->Settings.Size.x;

    float currentLength = 0.f;
    float width = GameplayUtil::RandomFloat(30.f, 50.f);

    do {
        const float height = GameplayUtil::PerlinNoise(Vector2(currentLength, 10)) * 10.f;

        Block* terrain = world->SpawnActor<Block>();

        terrain->SetSize(Vector2(width, height));
        terrain->SetPosition(Vector2(
            currentLength,
            height
        ));

        terrain->SetSimulatingPhysics(true);
        terrain->SetCollisionType(CollisionType::Block);
        terrain->SetMovability(ActorMovability::Static);
        terrain->SetUsingSimpleTexture(true);
        terrain->simpleColorFill = true;
        terrain->simpleColor = Color::GREEN();
        terrain->SetZIndex(1);

        currentLength += width;

        width = std::min(GameplayUtil::RandomFloat(10.f, 30.f), maxLength - currentLength);

    } while (currentLength < maxLength);

    currentLength = 0.f;

    do {
        const float height = GameplayUtil::PerlinNoise(Vector2(currentLength, 10)) * 50.f;

        Block* terrain = world->SpawnActor<Block>();

        terrain->SetSize(Vector2(width, height));
        terrain->SetPosition(Vector2(
            currentLength,
            height
        ));

        terrain->SetSimulatingPhysics(true);
        terrain->SetCollisionType(CollisionType::None);
        terrain->SetMovability(ActorMovability::Static);
        terrain->SetUsingSimpleTexture(true);
        terrain->simpleColorFill = true;
        terrain->simpleColor = Color(0xff'22'8B'22);

        currentLength += width;

        width = std::min(GameplayUtil::RandomFloat(10.f, 30.f), maxLength - currentLength);

    } while (currentLength < maxLength);


}

void PlatformerGamemode::computeWork() {
    constexpr uint64_t iterations = 2'000'000'000ULL;

    volatile uint64_t x = 0;

    for (uint64_t i = 0; i < iterations; ++i) {
        // Actual arithmetic work
        x = x * 1664525ULL + 1013904223ULL;
        x ^= x >> 13;
        x *= 0x5bd1e995ULL;
    }
}

void PlatformerGamemode::work() {
    const float res = 1;
    const int scale = 10;
    const int edgeSize = scale/res;

    const Vector2 startPos(50,100);

    World* world = GameInstance::Get()->GetWorld();

    Block* Grid[edgeSize][edgeSize];

    for (int i = 0; i < edgeSize; i++) {
        for (int j = 0; j < edgeSize; j++) {

            Block* b = world->SpawnActor<Block>();
            const Vector2 pos(
                (i*res),
                edgeSize-(j*res)
            );

            float out = GameplayUtil::PerlinNoise(pos, 0.1);

            b->SetPosition(pos + startPos);
            b->SetSize(Vector2(res,res));
            b->SetSimulatingPhysics(true);
            b->SetCollisionType(CollisionType::Block);
            b->SetMovability(ActorMovability::Movable);
            b->SetMass(0.1);
            b->SetUsingSimpleTexture(true);
            b->simpleColorFill = true;
            b->simpleColor = Color(
                out * 255,
                0x00,
                0x00
            );

        }
    }
}