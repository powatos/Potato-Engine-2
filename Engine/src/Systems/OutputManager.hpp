/** @file "OutputManager.hpp" */
#pragma once

#include <unordered_map>

#include "Core/EngineSubsystem.hpp"
#include "Core/InputController.hpp"
#include "Core/IScreenController.hpp"
#include "Core/Tickable.hpp"


struct WidgetMapper;
class Widget;

class OutputManager : public EngineSubsystem<OutputManager>, public IScreenController, public Tickable
{
    ENGINE_SUBSYSTEM(OutputManager)
public:
    virtual void Resolve() noexcept override;
    virtual void BeginPlay() override;

    void RegisterWidget(Widget* widget);
    void RemoveWidget(std::string UID);

    virtual void SetScreenResolution(const Vector2& resolution) override;
    virtual void SetWindowSize(const Vector2& size) override;
    virtual void SetWindowMode(WindowMode mode) override;
    virtual void SetRescaleMode(WindowRescaleMode mode) override;
    virtual void SetShowBorder(bool show) override;
    virtual void SetIsResizable(bool isResizable) override;

private:
    OutputManager();
    ~OutputManager();

    void DrawLevel();
    void DrawHUD();
    void DrawBars();

protected:
    void Draw();

    virtual void Tick(float dt) override;
    virtual void _TickRender(float dt) override;

    std::unordered_map<std::string, WidgetMapper*> WidgetMaps;

    void* MainWindow;
    void* Renderer;

};

struct WidgetMapper {
    Widget* widget;
    void* window;

    WidgetMapper(Widget* widget, void* window) : widget(widget), window(window) {}
};