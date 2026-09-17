/** @file UIRenderer.hpp */
#pragma once

struct Vector2;
struct Widget;
struct TextElement;
struct BoxElement;
struct ProgressBarElement;

namespace Internal
{

#define RenderSignature(UIClass) \
const class UIClass* ui, Vector2 uiPos, Vector2 uiSize

/**
 * @internal
 * @def DEFINE_RENDER_PASSTHROUGH
 * @brief Defines passthrough for UI rendering in UIElement classes
 * @details Call this macro at the start of widget element declarations that require custom rendering
 * @endinternal
 */
#define DEFINE_RENDER_PASSTHROUGH \
inline virtual void Internal_RenderPassthrough(Internal::UIRenderer* r, Vector2 uiPos, Vector2 uiSize) override { r->Render(this, uiPos, uiSize); }

    /**
     * @brief Internal low level rendering interface for UI objects
     */
    class UIRenderer
    {
    public:
        virtual ~UIRenderer() = default;

        virtual void Render(RenderSignature(Widget)) = 0;

        virtual void Render(RenderSignature(TextElement)) = 0;
        virtual void Render(RenderSignature(BoxElement)) = 0;
        virtual void Render(RenderSignature(ProgressBarElement)) = 0;

    protected:
        UIRenderer() = default;
    };

#undef RenderSignature

}