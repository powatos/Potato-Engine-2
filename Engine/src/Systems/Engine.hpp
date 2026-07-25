/** @file Engine.hpp */
#pragma once

#include "Core/EngineSubsystem.hpp"
#include "Core/EventDelegate.hpp"
#include "Core/Singleton.hpp"

class Engine : public EngineSubsystem<Engine>
{
    ENGINE_SUBSYSTEM(Engine)
    
public:
    virtual void Resolve() noexcept override;
    virtual void BeginPlay() override;

    int main();

private:
    Engine();
    ~Engine();

    void FireTick(const float dt) const;
    void FireTickPostPhysics(const float dt) const;

protected:
    std::vector<EventDelegate<float>> TickDelegates;

};