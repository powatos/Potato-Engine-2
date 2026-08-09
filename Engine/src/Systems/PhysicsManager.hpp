/** @file PhysicsManager.hpp */
#pragma once

#include "Core/Singleton.hpp"
#include "Core/EngineSubsystem.hpp"
#include "Core/Tickable.hpp"

#include "Util/Vector2.hpp"

class Actor;

class PhysicsManager : public EngineSubsystem<PhysicsManager>, public Tickable
{
    ENGINE_SUBSYSTEM(PhysicsManager)

public:
    virtual void Resolve() noexcept override;

protected:
    PhysicsManager();
    ~PhysicsManager();

    virtual void _TickPhysics(float dt) override;

private:
    struct SweepResult
    {
        bool collided = false;
        float toi = 1.f;
        Vector2 normal;
        Vector2 overlap;
    };
    struct SweepPair
    {
        Actor* a;
        Actor* b;
        float toi;
        Vector2 normal;
        Vector2 overlap;
        bool isAxisX;
        bool bothBlocking;
    };
    struct AxisTOI {float x = 1.f; float y = 1.f;};

    void UpdateActorVelocity(Actor* actor, float dt);
    
    SweepResult SweptAABB(
        const Vector2& aPos, const Vector2 bPos,
        const Vector2& aSize, const Vector2& bSize,
        const Vector2& relativeDisplacement
    ) const;
    
    void ApplyRestitutionImpulse(Actor* a, Actor* b, const Vector2& normal);
    void ApplyPenetrationCorrection(Actor* a, Actor* b, const Vector2& normal, float penetration);
};