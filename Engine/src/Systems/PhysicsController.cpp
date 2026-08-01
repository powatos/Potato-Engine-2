/** @file PhysicsController.cpp */

#include <limits>
#include <unordered_map>

#include "Game/Actor.hpp"
#include "Core/GameInstance.hpp"
#include "Game/HitResult.hpp"

#include "Util/GameplayHelper.hpp"
#include "Debug/Log.hpp"

#include "PhysicsController.hpp"

PhysicsController::PhysicsController() {
    LOG(LogType::VITAL, "PhysicsController constructed");
}

void PhysicsController::_TickPhysics(float dt) {
    const ActorPool& actorPool = GameInstance::Get()->GetWorld()->GetAllActors();
    constexpr float TOI_EPSILON = 1e-4f;

    // update velocity for each actor
    for (Actor* actor : actorPool) {
        if (actor == nullptr) { continue; }

        if (!actor->isSimulatingPhysics()) { continue; }
        if (actor->GetMovability() == ActorMovability::Static) { continue; }

        UpdateActorVelocity(actor, dt);
    }

    // sweep every pair
    std::vector<SweepPair> pendingContacts;
    std::unordered_map<Actor*, AxisTOI> nearestAxisTOI;

    auto UpdateNearestAxis = [&](Actor* actor, bool isAxisX, float toi) {
        AxisTOI& entry = nearestAxisTOI[actor];
        const float clamped = std::max(toi, 0.f);

        if (isAxisX) { entry.x = std::min(entry.x, clamped); }
        else { entry.y = std::min(entry.y, clamped); }
    };

    for (Actor* a : actorPool) {
        if (a->GetCollisionType() == CollisionType::None) { continue; }

        for (Actor* b : actorPool) {
            if (a >= b) { continue; } // pointer trick to avoid checking pairs twice and against self
            if (b->GetCollisionType() == CollisionType::None) { continue; }

            const Vector2 aPos = a->GetPosition();
            const Vector2 bPos = b->GetPosition();
            const Vector2 relativeDisplacement = (a->GetVelocity() - b->GetVelocity()) * dt;

            // broadphase check against actors farther than 10'000.f + (displacement) units
            const float broadphaseRadius = 10'000.f + relativeDisplacement.Magnitude();
            if (aPos.SquaredDistance(bPos) >= broadphaseRadius * broadphaseRadius) { continue; }

            const SweepResult sweep = SweptAABB(aPos, bPos, a->GetSize(), b->GetSize(), relativeDisplacement);
            if (!sweep.collided) { continue; }

            const bool bothBlocking = (a->GetCollisionType() == CollisionType::Block) && (b->GetCollisionType() == CollisionType::Block);
            const bool isAxisX = sweep.normal.x != 0.f;

            pendingContacts.push_back({a, b, sweep.toi, sweep.normal, sweep.overlap, isAxisX, bothBlocking});

            if (bothBlocking) {

                bool skipClamp = false;
                if (sweep.toi <= 0.f) {
                    const float normalVel = (a->GetVelocity() - b->GetVelocity()).Dot(sweep.normal);
                    skipClamp = (normalVel > 0.f);
                }

                if (!skipClamp) {
                    UpdateNearestAxis(a, isAxisX, sweep.toi);
                    UpdateNearestAxis(b, isAxisX, sweep.toi);
                }

            }
        }
    }

    auto GetAxisTOI = [&](Actor* actor, bool isAxisX) -> float {
        auto it = nearestAxisTOI.find(actor);
        if (it == nearestAxisTOI.end()) { return 1.f; }
        return isAxisX ? it->second.x : it->second.y;
    };

    auto getAxisTOI = [&](Actor* actor, bool isAxisX) -> float {
        if (actor->GetMovability() == ActorMovability::Static || !actor->isSimulatingPhysics()) {
            return std::numeric_limits<float>::infinity();
        }
        return GetAxisTOI(actor, isAxisX);

    };

    auto OwnStoppingAxisTOI = [&](Actor* actor, bool axisIsX) -> float {
        if (actor->GetMovability() == ActorMovability::Static || !actor->isSimulatingPhysics()) {
            return std::numeric_limits<float>::infinity();
        }
        return GetAxisTOI(actor, axisIsX);
    };

    for (Actor* actor : actorPool) {
        if (!actor->isSimulatingPhysics()) { continue; }
        if (actor->GetMovability() == ActorMovability::Static) { continue; }

        auto it = nearestAxisTOI.find(actor);
        const AxisTOI toi = (it != nearestAxisTOI.end()) ? it->second : AxisTOI{};
        const Vector2 vel = actor->GetVelocity();
        actor->AddLocalOffset(Vector2(vel.x * toi.x, vel.y * toi.y) * dt);
    }

    for (const SweepPair& contact : pendingContacts) {
        const bool aStoppedEarlier = OwnStoppingAxisTOI(contact.a, contact.isAxisX) < contact.toi - TOI_EPSILON;
        const bool bStoppedEarlier = OwnStoppingAxisTOI(contact.b, contact.isAxisX) < contact.toi - TOI_EPSILON;
        if (aStoppedEarlier || bStoppedEarlier) { continue; }

        if (contact.bothBlocking) {
            if (contact.toi <= 0.f && contact.overlap.x > 0.f && contact.overlap.y > 0.f) {
                const float penetration = contact.isAxisX ? contact.overlap.x : contact.overlap.y;
                ApplyPenetrationCorrection(contact.a, contact.b, contact.normal, penetration);
            }

            ApplyRestitutionImpulse(contact.a, contact.b, contact.normal);
        }

        const Vector2 aPos = contact.a->GetPosition();
        const Vector2 bPos = contact.b->GetPosition();
        const Vector2 aSize = contact.a->GetSize();
        const Vector2 bSize = contact.b->GetSize();

        HitResult hitResult;
        hitResult.distance = 0.f;
        hitResult.hitNormal = contact.normal;
        hitResult.hitOverlap = Vector2(
            std::min(aPos.x + aSize.x, bPos.x + bSize.x) - std::max(aPos.x, bPos.x),
            std::min(aPos.y, bPos.y) - std::max(aPos.y - aSize.y, bPos.y - bSize.y)
        );
        hitResult.hitPosition = Vector2(std::max(aPos.x, bPos.x), std::min(aPos.y, bPos.y));

        HitResult hitResultA = hitResult;
        hitResultA.hitActor = contact.b;
        contact.a->OnHit(hitResultA);

        HitResult hitResultB = hitResult;
        hitResultB.hitActor = contact.a;
        hitResultB.hitNormal = -contact.normal;
        contact.b->OnHit(hitResultB);
    }

}

void PhysicsController::UpdateActorVelocity(Actor* actor, float dt) {
    const World::WorldSettings& Settings = GameInstance::Get()->GetWorld()->Settings;

    if (actor == nullptr) { return; }

    Vector2 forces = actor->GetForces();
    if (Settings.doGravity) {
        if (actor->IsUsingAsymmetricGravity() && actor->GetVelocity().Dot(Vector2::Up()) > 0.f) {
            forces += Vector2(0, -Settings.upGravity);
        } else {
            forces += Vector2(0, -Settings.downGravity);
        }
    }

    const Vector2 acceleration = forces / actor->GetMass();
    const Vector2 dVelocity = acceleration * dt;

    actor->AddImpulse(dVelocity);
    actor->ClearForces();
}

PhysicsController::SweepResult PhysicsController::SweptAABB(
    const Vector2& aPos, const Vector2 bPos,
    const Vector2& aSize, const Vector2& bSize,
    const Vector2& relativeDisplacement
) const
{
    SweepResult result;

    const Vector2 aMin(aPos.x, aPos.y - aSize.y);
    const Vector2 aMax(aPos.x + aSize.x, aPos.y);
    const Vector2 bMin(bPos.x, bPos.y - bSize.y);
    const Vector2 bMax(bPos.x + bSize.x, bPos.y);

    Vector2 entryTime, exitTime;

    if (relativeDisplacement.x > 0.f) {
        entryTime.x = (bMin.x - aMax.x) / relativeDisplacement.x;
        exitTime.x  = (bMax.x - aMin.x) / relativeDisplacement.x;
    } else if (relativeDisplacement.x < 0.f) {
        entryTime.x = (bMax.x - aMin.x) / relativeDisplacement.x;
        exitTime.x  = (bMin.x - aMax.x) / relativeDisplacement.x;
    } else if (aMax.x > bMin.x && aMin.x < bMax.x) {
        entryTime.x = -std::numeric_limits<float>::infinity();
        exitTime.x  =  std::numeric_limits<float>::infinity();
    } else {
        entryTime.x =  std::numeric_limits<float>::infinity();
        exitTime.x  = -std::numeric_limits<float>::infinity();
    }

    if (relativeDisplacement.y > 0.f) {
        entryTime.y = (bMin.y - aMax.y) / relativeDisplacement.y;
        exitTime.y  = (bMax.y - aMin.y) / relativeDisplacement.y;
    } else if (relativeDisplacement.y < 0.f) {
        entryTime.y = (bMax.y - aMin.y) / relativeDisplacement.y;
        exitTime.y  = (bMin.y - aMax.y) / relativeDisplacement.y;
    } else if (aMax.y > bMin.y && aMin.y < bMax.y) {
        entryTime.y = -std::numeric_limits<float>::infinity();
        exitTime.y  =  std::numeric_limits<float>::infinity();
    } else {
        entryTime.y =  std::numeric_limits<float>::infinity();
        exitTime.y  = -std::numeric_limits<float>::infinity();
    }

    const float finalEntry = std::max(entryTime.x, entryTime.y);
    const float finalExit  = std::min(exitTime.x, exitTime.y);

    if (finalEntry > finalExit
        || finalExit < 0.f
        || finalEntry > 1.f
    ) { return result; }

    result.collided = true;
    result.toi = std::max(0.f, finalEntry);

    result.overlap.x = std::min(aMax.x, bMax.x) - std::max(aMin.x, bMin.x);
    result.overlap.y = std::min(aMax.y, bMax.y) - std::max(aMin.y, bMin.y);

    if (result.toi <= 0) {
        const Vector2 aCenter((aMin.x + aMax.x) * 0.5f, (aMin.y + aMax.y) * 0.5f);
        const Vector2 bCenter((bMin.x + bMax.x) * 0.5f, (bMin.y + bMax.y) * 0.5f);
        const float dx = bCenter.x - aCenter.x;
        const float dy = bCenter.y - aCenter.y;

        if (result.overlap.x < result.overlap.y) {
            result.normal = Vector2(dx > 0.f ? -1.f : 1.f, 0.f);
        } else {
            result.normal = Vector2(0.f, dy > 0.f ? -1.f : 1.f);
        }
    } else {

        if (entryTime.x > entryTime.y) {
            result.normal = Vector2(relativeDisplacement.x > 0.f ? -1.f : 1.f, 0.f);
        } else {
            result.normal = Vector2(0.f, relativeDisplacement.y > 0.f ? -1.f : 1.f);
        }
        
    }

    return result;
}

void PhysicsController::ApplyRestitutionImpulse(Actor* a, Actor* b, const Vector2& normal) {
    const World::WorldSettings& Settings = GameInstance::Get()->GetWorld()->Settings;

    const float aInvMass = a->GetMovability() == ActorMovability::Static ? 0.f : (1.f / a->GetMass());
    const float bInvMass = b->GetMovability() == ActorMovability::Static ? 0.f : (1.f / b->GetMass());
    const float totalInvMass = aInvMass + bInvMass;
    if (totalInvMass <= 0.f) { return; }

    const Vector2 relativeVel = a->GetVelocity() - b->GetVelocity() ;
    const float normalVel = relativeVel.Dot(normal);
    if (normalVel >= 0.f) { return; }

    float restitution = a->GetBounce() * b->GetBounce();
    if (std::abs(normalVel) < Settings.bounceThreshold) { restitution = 0.f; }
    
    const float impulseMag = -(1.f + restitution) * normalVel / totalInvMass;
    const Vector2 impulse = normal * impulseMag;

    a->AddImpulse(impulse * aInvMass);
    b->AddImpulse(-impulse * bInvMass);
}

void PhysicsController::ApplyPenetrationCorrection(Actor* a, Actor* b, const Vector2& normal, float penetration) {
    const World::WorldSettings& Settings = GameInstance::Get()->GetWorld()->Settings;

    const float seperationAmount = std::max(penetration - Settings.clipAllowed, 0.f) * Settings.clipDampeningFactor;
    if (seperationAmount <= 0) { return; }

    const Vector2 correctionVector = normal * seperationAmount;

    const float aInvMass = a->GetMovability() == ActorMovability::Static ? 0.f : (1.f / a->GetMass());
    const float bInvMass = b->GetMovability() == ActorMovability::Static ? 0.f : (1.f / b->GetMass());
    const float totalInvMass = aInvMass + bInvMass;
    if (totalInvMass <= 0.f) { return; }

    a->AddLocalOffset(correctionVector * (aInvMass / totalInvMass));
    b->AddLocalOffset(-correctionVector * (bInvMass / totalInvMass));
}

void PhysicsController::Resolve() noexcept {
    LOG(LogType::VITAL, "Resolving PhysicsController");
}

PhysicsController::~PhysicsController() {
}