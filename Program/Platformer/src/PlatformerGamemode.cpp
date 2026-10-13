
#include "Core/GameInstance.hpp"
#include "Actors/Block.hpp"

#include "PlatformerGamemode.hpp"

#include "Core/InputController.hpp"
#include "UI/UIManager.hpp"
#include "UI/ProgressBarElement.hpp"
#include "Util/GameplayUtil.hpp"

SET_DEFAULT_SUBCLASS(Gamemode, PlatformerGamemode)

#define plat(px, py, sx, sy) platform(Vector2(px,py), Vector2(sx,sy));

void PlatformerGamemode::BeginPlay() {
    Gamemode::BeginPlay();

    plat(10,10,20,1)
    plat(50,15,20,1)

    plat(80,25,20,25)

    auto j = platform(Vector2(45,55), Vector2(10,10));
    j->SetCollisionType(CollisionType::None);
    j->simpleColor = Color(0x00, 0xff, 0x00, 100u);

    plat(0,40,20,5)
    plat(0,56,17,5)

    plat(60, 80, 20, 1)
    plat(80, 85, 20, 1)

    auto f = platform(Vector2(85,100), Vector2(15,15));
    f->SetCollisionType(CollisionType::None);
    f->simpleColor = Color(0x00, 0x00, 0xff, 0xff);

}

Block* PlatformerGamemode::platform(const Vector2& pos, const Vector2& size) {
    World* world = GameInstance::Get()->GetWorld();
    auto b = world->SpawnActor<Block>(pos);
    b->SetSize(size);
    b->SetUsingSimpleTexture(true);
    b->simpleColor = Color::BLACK();
    b->simpleColorFill = true;

    return b;
}

#undef plat
