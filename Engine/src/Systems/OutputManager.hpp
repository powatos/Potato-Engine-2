/** @file "OutputManager.hpp" */
#pragma once

#include <unordered_map>

#include "Core/EngineSubsystem.hpp"
#include "Core/InputController.hpp"
#include "Core/Tickable.hpp"


struct WidgetMapper;
class Widget;

class OutputManager : public EngineSubsystem<OutputManager>, public Tickable
{
    ENGINE_SUBSYSTEM(OutputManager)
public:
    virtual void Resolve() noexcept override;
    virtual void BeginPlay() override;

    void RegisterWidget(Widget* widget);
    void RemoveWidget(std::string UID);

    float FRAMES_PER_SECOND;

private:
    OutputManager();
    ~OutputManager();

    void DrawLevel();
    void DrawHUD();

protected:
    void Draw();

    virtual void _TickRender(float dt) override;

    std::unordered_map<std::string, WidgetMapper*> WidgetMaps;

    void* DisplayWindow;

};

struct WidgetMapper {
    Widget* widget;
    void* window;

    WidgetMapper(Widget* widget, void* window) : widget(widget), window(window) {}
};