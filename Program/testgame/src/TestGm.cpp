/** @file "TestGm.cpp" */


#include "Core/GameInstance.hpp"
#include "Game/Block.hpp"

#include "TestGm.hpp"

#include "Util/GameplayUtil.hpp"

SET_DEFAULT_SUBCLASS(Gamemode, TestGM)

void TestGM::BeginPlay() {
    Gamemode::BeginPlay();

    // work();


}

void TestGM::work() {
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