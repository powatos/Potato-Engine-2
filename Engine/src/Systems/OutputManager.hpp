/** @file "OutputManager.hpp" */
#pragma once

#include <unordered_map>

#include "Core/EngineSubsystem.hpp"
#include "Core/IWindowController.hpp"
#include "Core/Tickable.hpp"
#include "UI/____LLUIRenderer.hpp"

#include "UI/UIManager.hpp"

class OutputManager : public EngineSubsystem<OutputManager>, public IWindowController, public Tickable, public ___LLUIRenderer
{
    ENGINE_SUBSYSTEM(OutputManager)
public:
    virtual void Resolve() noexcept override;
    virtual void BeginPlay() override;

    virtual void* RequestRenderingContext() const override;
    virtual void* RequestTTFEngine() const override;

    virtual void SetScreenResolution(const Vector2& resolution) override;
    virtual void SetWindowSize(const Vector2& size) override;
    virtual void SetWindowMode(WindowMode mode) override;
    virtual void SetRescaleMode(WindowRescaleMode mode) override;
    virtual void SetShowBorder(bool show) override;
    virtual void SetIsResizable(bool isResizable) override;

    virtual void Render(const class Widget* ui, UIVector posScale, UIVector sizeScale) override;
    virtual void Render(const class TextElement* ui, UIVector posScale, UIVector sizeScale) override;
    virtual void Render(const class BoxElement* ui, UIVector posScale, UIVector sizeScale) override;

private:
    OutputManager();
    ~OutputManager();

    void DrawLevel();
    void DrawHUD();
    void Recurse_DrawUI(const UIHierarchy* ui, UIVector posScale, UIVector sizeScale);
    void DrawBars();

    Vector2 getScaledVec(const UIVector& vec, const UIVector& scale) const;

protected:
    void Draw();

    virtual void Tick(float dt) override;
    virtual void _TickRender(float dt) override;

    void* MainWindow;
    void* Renderer;

    void* TextEngine;

};
