/** @file Raycast.cpp */

#include <algorithm>
#include <utility>
#include <cfloat>

#include "Util/TimerManager.hpp"
#include "Core/GameInstance.hpp"
#include "Game/Block.hpp"
#include "Core/TickController.hpp"

#include "Debug/Log.hpp"

#include "Game/Raycast.hpp"

Raycast::Raycast() {
    LOG(LogType::WARNING, "Empty raycast constructed");
    showDebug = false;
}

Raycast::Raycast(const Vector2& origin, const Vector2& ray, RaycastSettings Settings) : origin(origin), Settings(Settings), ray(ray) {
    rayLength = ray.Magnitude();
}

Block* Raycast::Cast(HitResult& outHit) {
    World* world = GameInstance::Get()->GetWorld();
    const std::unordered_set<Actor*> OverrideActorsList = Settings.OverrideActorsList;

    outHit.ray_param = FLT_MAX;
    
    for (Actor* actor : world->GetAllActors()) {
        if (actor->GetCollisionType() == CollisionType::None) { continue; }

        switch ( Settings.OverrideActorsType ) {
            case RaycastOverrideType::Exclude:
                // if exclude and current actor should be excluded, continue
                if (OverrideActorsList.find(actor) != OverrideActorsList.end()) { continue; }
                break;
            case RaycastOverrideType::Include:
                // if include and current actor should not be included, continue
                if (OverrideActorsList.find(actor) == OverrideActorsList.end()) { continue; }
                break;
            default:
                // NoOverride
                break;
        }

        HitResult hit;
        ComputeRaycast(actor, hit);

        if (hit.hitActor != nullptr && hit.ray_param < outHit.ray_param) {
            outHit = hit;
        }

    }

    if (outHit.hitActor != nullptr) {
        outHit.distance = outHit.ray_param * ray.Magnitude();
    }

    Block* trail = nullptr;

    // TODO: line
    // if (showDebug) {
    //     trail = world->SpawnActor<Block>(origin);
    //     trail->SetSize(Vector2(outHit.distance == 0 ? ray.Magnitude() : outHit.distance, 1));
    //     trail->SetCollisionType(CollisionType::None);
    //     trail->SetSimulatingPhysics(false);
    //     trail->GetTexture().SetRotation(ray.Angle());
    //     trail->ctex = 'r';
    //     trail->SetVisibility(true);
    //
    //     TimerManager::Get()->AddTimer("raycast_trail_delete_delay", debugDuration, world, &World::DestroyActor, trail);
    //     TickController::Get()->Unregister(trail); // avoid uneccesary tick checks
    //
    // }

    return trail;
}

void Raycast::ComputeRaycast(Actor* actor, HitResult& outHit) {
    outHit.hitActor = nullptr;

    const Vector2 actorPos = actor->GetPosition();
    const Vector2 boxMax = actorPos + actor->GetSize() * Vector2(1.f,-1.f);

    Vector2 normal;
    
    float tMin = 0.f;
    float tMax = 1.f;

    if (ray.x != 0.f) {
        float tx1 = (actorPos.x - origin.x) / ray.x;
        float tx2 = (boxMax.x - origin.x) / ray.x;  

        Vector2 normalX;

        if (tx1 < tx2) {
            normalX = Vector2{-1.f, 0.f};
        } else {
            std::swap(tx1, tx2);
            normalX = Vector2{1.f, 0.f};
        }

        if (tx1 > tMin) {
            tMin = tx1;
            normal = normalX;
        }

        tMax = std::min(tMax, tx2);

        if (tMin > tMax) { return; }

    } else if (origin.x < actorPos.x || origin.x > boxMax.x) { return; }

    if (ray.y != 0.f) {
        float ty1 = (actorPos.y - origin.y) / ray.y;
        float ty2 = (boxMax.y - origin.y) / ray.y;
        
        Vector2 normalY;

        if (ty1 < ty2) {
            normalY = Vector2{0.f, -1.f};
        } else {
            std::swap(ty1, ty2);
            normalY = {0.f, 1.f};
        }

        if (ty1 > tMin) {
            tMin = ty1;
            normal = normalY;
        }

        tMax = std::min(tMax, ty2);

        if (tMin > tMax) { return; }

    } else if (origin.y > actorPos.y || origin.y < boxMax.y) { return; }

    
    
    outHit.hitActor = actor;
    outHit.hitPosition = origin + ray * tMin;
    outHit.hitNormal = normal;
    outHit.ray_param = tMin;

}

void Raycast::SetRay(const Vector2& newRay) {
    ray = newRay;
    rayLength = ray.Magnitude();
}