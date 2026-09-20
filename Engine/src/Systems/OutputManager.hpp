/** @file "OutputManager.hpp" */
#pragma once

#include <unordered_map>

#include "Core/EngineSubsystem.hpp"
#include "Core/IWindowController.hpp"
#include "Core/Tickable.hpp"
#include "UI/UIRenderer.hpp"

#include "UI/UIManager.hpp"

class OutputManager : public EngineSubsystem<OutputManager>, public IWindowController, public Tickable, public Internal::UIRenderer
{
    ENGINE_SUBSYSTEM(OutputManager)
public:
    virtual void Resolve() noexcept override;
    virtual void BeginPlay() override;

    virtual void* RequestRenderingContext() const override;
    virtual void* RequestTTFEngine() const override;

    void ShowBootstrapScreen() override;
    void HideBootstrapScreen() override;

    virtual void SetScreenResolution(const Vector2& resolution) override;
    virtual void SetWindowSize(const Vector2& size) override;
    virtual void SetWindowMode(WindowMode mode) override;
    virtual void SetRescaleMode(WindowRescaleMode mode) override;
    virtual void SetShowBorder(bool show) override;
    virtual void SetIsResizable(bool isResizable) override;

    virtual void Render(const struct Widget* ui, Vector2 uiPos, Vector2 uiSize) override;
    virtual void Render(const struct TextElement* ui, Vector2 uiPos, Vector2 uiSize) override;
    virtual void Render(const struct BoxElement* ui, Vector2 uiPos, Vector2 uiSize) override;
    virtual void Render(const struct ProgressBarElement* ui, Vector2 uiPos, Vector2 uiSize) override;

private:
    OutputManager();
    ~OutputManager();

    bool bootstrapScreenEnabled;

    void DrawLevel();
    void DrawDebugObjects();
    void DrawHUD();
    void Recurse_DrawUI(const UIHierarchy* ui, Vector2 parentAbsPos, Vector2 parentAbsSize);
    void DrawBars();

protected:
    void Draw();

    virtual void Tick(float dt) override;
    virtual void _TickRender(float dt) override;

    void* MainWindow;
    void* Renderer;

    void* TextEngine;

};
