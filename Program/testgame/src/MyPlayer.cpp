
#include <Core/GameInstance.hpp>

#include <Debug/Log.hpp>

#include "MyPlayer.hpp"

#include "Core/InputController.hpp"
#include "Game/Raycast.hpp"

SET_DEFAULT_SUBCLASS(Player, MyPlayer)

MyPlayer::MyPlayer() : downraycast(Raycast()) {
    SetBounce(1.f);
}

void MyPlayer::BeginPlay() {
    downraycast.SetRay(Vector2(5,-5));
    downraycast.Settings.OverrideActorsList.insert(this);
    downraycast.Settings.OverrideActorsType = RaycastOverrideType::Exclude;
    downraycast.showDebug = false;
    downraycast.debugDuration = 1.0;

    const auto IC = PotatoEngine::Get().GetInputController();
    IC->RegisterInputBinding({
        InputBinding(Keycode::A, InputType::Started, "aStart", []{LOG(LogType::INFO, "a start");}),
        InputBinding(Keycode::A, InputType::Ongoing, "aOngoing", []{LOG(LogType::INFO, "a ongoing");}),
        InputBinding(Keycode::A, InputType::Completed, "aComplete", []{LOG(LogType::INFO, "a completed");})
    });

}

void MyPlayer::Tick(float dt) {
    // HitResult hit;
    // downraycast.origin = GetPosition() + Vector2::Right() * GetSize().x;
    // downraycast.Cast(hit);

    // // if (hit.hitActor) {
    // //     LOG(LogType::DEBUG, "{}", hit.distance);
    // // }

    // LOG(LogType::DEBUG, "{}", GetPosition().ToStringF());
}