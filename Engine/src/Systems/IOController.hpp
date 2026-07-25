/** @file IOController.hpp */
#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

#include "Core/EngineSubsystem.hpp"
#include "Core/Tickable.hpp"
#include "Core/InputController.hpp"
#include "Core/Singleton.hpp"

using BindingMap = std::unordered_map<Keycode, std::vector<InputBinding>, __KeycodeHash>;
struct WidgetMapper;
class Widget;

class IOController : public EngineSubsystem<IOController>, public IInputController, public Tickable
{
    ENGINE_SUBSYSTEM(IOController)

public:
    virtual void Resolve() noexcept override;
    virtual void BeginPlay() override;

    void RegisterWidget(Widget* widget);
    void RemoveWidget(std::string UID);

    virtual void RegisterInputBinding(InputBinding binding) override;
    virtual void RegisterInputBinding(std::initializer_list<InputBinding> bindings) override;
    virtual void UnregisterInputBinding(std::string deleteName) override;
    virtual void UnregisterAllInputBindings(void* object) override;

    float FRAMES_PER_SECOND;
    int MS_REPEAT_THRESHOLD;

private:
    IOController();
    ~IOController();

    void UnregisterBindingFrom(BindingMap&, std::string deleteName);
    void UnregisterAllBindingsFrom(BindingMap&, void* object);
    void FireBinding(BindingMap&, Keycode key);

    void DrawLevel();
    void DrawHUD();
    
protected:
    void HandleInput();
    void Draw();

    virtual void _TickInput(float dt) override;
    virtual void _TickRender(float dt) override;

    void* BoxWindow;
    void* DisplayWindow;

    std::unordered_map<std::string, WidgetMapper*> WidgetMaps;

    BindingMap InputBindingsTriggered;
    BindingMap InputBindingsOngoing;
    BindingMap InputBindingsCompleted;
    BindingMap InputBindingsImpulse;

    Keycode ActiveKey;
    Keycode ImpulseKey;
    
};

struct WidgetMapper {
    Widget* widget;
    void* window;

    WidgetMapper(Widget* widget, void* window) : widget(widget), window(window) {}
};