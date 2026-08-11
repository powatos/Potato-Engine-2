/** @file Actor.cpp */

#include <algorithm>

#include "Core/GameInstance.hpp"
#include "Core/EventManager.hpp"
#include "Core/PotatoEngine.hpp"
#include "Game/World.hpp"

#include "Debug/Log.hpp"
#include "Util/Vector2.hpp"

#include "Game/Actor.hpp"

#include "Core/TextureManager.hpp"

ARCHIVE_STATIC(Actor)

Actor::Actor()
{

    Position = Vector2();
    Size = Vector2();
    Velocity = Vector2();

    Bounciness = 0.f;
    Mass = 10.f;
    SimulatePhysics = true;
    UseAsymmetricGravity = false;
    Movability = ActorMovability::Movable;
    CollisionResponse = CollisionType::Block;

    bUseSimpleTexture = false;
    simpleColorFill = false;
    simpleColor = Color{};

    ActorTexture = TextureManager::Get()->GetTexture(""); // fallbacks to default texture

    isInPlay = false;
    Visible = true;

}

void Actor::DispatchBeginPlay() {
    if (isInPlay) { return; }
    isInPlay = true;

    BeginPlay();
}

void Actor::BeginPlay() {
    // if (!bUseSimpleTexture && !ActorTexture) {
    //     // if using Texture but it's invalid
    //     LOG(LogType::WARNING, "Actor texture invalid");
    // }

}

void Actor::Tick(float dt) {
    Tickable::Tick(dt);
}

void Actor::OnHit(const HitResult& hitResult) {
    // LOG(LogType::DEBUG, "Actor hit at {} with {} overlap", hitResult.hitPosition.ToString(), hitResult.hitOverlap.ToString());
}

Actor::~Actor() {

}

#pragma region Accessors and Mutators

Vector2 Actor::GetPosition() const { 
    return Position; 
}
void Actor::SetPosition(const Vector2 &position) { 
    Position = position;
}
void Actor::AddLocalOffset(const Vector2& offset) {
    SetPosition(Position + offset);
}

Vector2 Actor::GetSize() const { 
    return Size; 
}
void Actor::SetSize(const Vector2 &size) { 
    Size = size;
}
void Actor::ResizeToTexture() {
    // Size = ActorTexture.GetBoundingBox();
}

bool Actor::isVisible() const {
    return Visible;
}
void Actor::SetVisibility(bool visibility) {
    Visible = visibility;
}

Vector2 Actor::GetVelocity() const { 
    return Velocity; 
}
void Actor::SetVelocity(const Vector2& velocity) { 
    Velocity = velocity;
}
void Actor::AddImpulse(const Vector2& force) {
    Velocity += force;
}

float Actor::GetMass() const {
    return Mass;
}
void Actor::SetMass(float mass) {
    Mass = mass;
}

bool Actor::isSimulatingPhysics() const {
    return SimulatePhysics;
}
void Actor::SetSimulatingPhysics(bool enabled) {
    SimulatePhysics = enabled;
}

Vector2 Actor::GetForces() const {
    return Forces;
}
void Actor::AddForce(const Vector2& force) {
    Forces += force;
}
void Actor::ClearForces() {
    Forces = Vector2(0.f, 0.f);
}

bool Actor::IsUsingAsymmetricGravity() const {
    return UseAsymmetricGravity;
}
void Actor::SetUseAsymmetricGravity(bool useAsymmetricGravity) {
    UseAsymmetricGravity = useAsymmetricGravity;
}

ActorMovability Actor::GetMovability() const {
    return Movability;
}
void Actor::SetMovability(ActorMovability movability) {
    Movability = movability;
}

CollisionType Actor::GetCollisionType() const {
    return CollisionResponse;
}
void Actor::SetCollisionType(CollisionType collisionType) {
    CollisionResponse = collisionType;
}

float Actor::GetBounce() const {
    return Bounciness;
}
void Actor::SetBounce(float bounce) {
    Bounciness = bounce;
}

Texture* Actor::GetTexture() const {
    return ActorTexture;
}
void Actor::SetTexture(const std::string& texturePath) {
    ActorTexture = TextureManager::Get()->GetTexture(texturePath);
}
void Actor::SetTexture(Texture* texture) {
    ActorTexture = texture;
}

bool Actor::IsUsingSimpleTexture() const {
    return bUseSimpleTexture;
}
void Actor::SetUsingSimpleTexture(bool enabled) {
    bUseSimpleTexture = enabled;
}

#pragma endregion


