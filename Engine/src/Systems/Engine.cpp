/** @file Engine.cpp */

#include <chrono>
#include <thread>

#include "Core/GameInstance.hpp"
#include "Game/World.hpp"
#include "Systems/IOController.hpp"
#include "Core/TickController.hpp"

#include "Debug/Log.hpp"

#include "Engine.hpp"

Engine::Engine() {

    LOG(LogType::VITAL, "Engine constructed");

}

void Engine::BeginPlay() {

}

int Engine::main() {
    namespace stdc = std::chrono;
    using ms = stdc::milliseconds;

    LOG(LogType::VITAL, "Engine main loop started");
    
    const IOController* Controller = IOController::Get();
    const GameInstance* Instance = GameInstance::Get();
    TickController* tickController = TickController::Get();

    const ms idealDelay(static_cast<int>(1000.f / Controller->FRAMES_PER_SECOND));
    auto lastTick = stdc::steady_clock::now();

    while (Instance->_isMainTickRunning == true) {
        auto currentTick = stdc::steady_clock::now();

        const stdc::duration<float> elapsed = currentTick - lastTick;
        lastTick = currentTick;

        float dt = elapsed.count();

        if (dt >= 0.5f) { dt = 0.5f; } // clamp max dt

        {
            tickController->Fire(dt, TickGroup::PreInput);

            tickController->Fire(dt, TickGroup::_Input);

            tickController->Fire(dt, TickGroup::Update);
            tickController->Fire(dt, TickGroup::PostUpdate);

            tickController->Fire(dt, TickGroup::_Physics);

            tickController->Fire(dt, TickGroup::PostPhysics);

            tickController->Fire(dt, TickGroup::_Render);

            tickController->Fire(dt, TickGroup::PostRender);
        }

        auto end = stdc::steady_clock::now();
        auto tickDuration = stdc::duration_cast<ms>(end - currentTick);

        if (tickDuration >= idealDelay) { continue; } // no sleep if passed ideal tick time

        const auto correctionDelay = idealDelay - tickDuration;
        // account for sleep latency (only sleep if buffer is larger than 2ms)
        // if (correctionDelay > ms(2)){
        //     std::this_thread::sleep_for(correctionDelay - ms(1));
        // }

        // // spin cpu while waiting for delay to be reached
        // while (stdc::steady_clock::now() - currentTick < idealDelay) {
        //     #if defined(__x86_64__) || defined(__M_X64)
        //         _mm_pause();
        //     #endif    
        // }
        std::this_thread::sleep_for(correctionDelay);
        
    }
    
    LOG(LogType::VITAL, "Engine main loop finished");

    return 0;
}

void Engine::FireTick(const float dt) const {
    EventController::Get()->FireNativeEvent<float>("__ENGINE_TICK", dt);
}
void Engine::FireTickPostPhysics(const float dt) const {
    EventController::Get()->FireNativeEvent<float>("__ENGINE_TICK_POST_PHYSICS", dt);
}

void Engine::Resolve() noexcept {
    LOG(LogType::VITAL, "Resolving Engine");
}

Engine::~Engine() {
}