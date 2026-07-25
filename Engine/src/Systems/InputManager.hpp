/** @file "InputManager.hpp" */
#pragma once

#include "Core/EngineSubsystem.hpp"
#include "Core/Tickable.hpp"
#include "Core/InputController.hpp"

using BindingMap = std::unordered_map<Keycode, std::vector<InputBinding>, ___KeycodeHash>;

class InputManager : public EngineSubsystem<InputManager>, public IInputController, public Tickable
{
    ENGINE_SUBSYSTEM(InputManager)

public:
    virtual void Resolve() noexcept override;
    virtual void BeginPlay() override;

    virtual void RegisterInputBinding(InputBinding binding) override;
    virtual void RegisterInputBinding(std::initializer_list<InputBinding> bindings) override;
    virtual void UnregisterInputBinding(std::string deleteName) override;
    virtual void UnregisterAllInputBindings(void* object) override;

private:
    InputManager();
    ~InputManager();

    void UnregisterBindingFrom(BindingMap& map, std::string deleteName);
    void UnregisterAllBindingsFrom(BindingMap& map, void* object);
    void FireBinding(BindingMap& map, Keycode key);

    BindingMap InputBindingsStarted;
    BindingMap InputBindingsOngoing;
    BindingMap InputBindingsCompleted;
    BindingMap InputBindingsImpulse;

    std::vector<Keycode> CurrentActiveKeys;

protected:
    void HandleInput();

    virtual void _TickInput(float dt) override;
};