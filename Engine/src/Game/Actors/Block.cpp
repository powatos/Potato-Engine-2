/** @file Block.cpp */

#include "Actors/Block.hpp"

ARCHIVE_STATIC(Block)

Block::Block() {
    SetSize(Vector2(20, 5));
    SetMovability(ActorMovability::Static);

    SetUsingSimpleTexture(true);

}