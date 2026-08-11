/** @file UIRenderer.hpp */
#pragma once

struct UIVector;
struct Widget;
struct TextElement;
struct BoxElement;

namespace Internal
{

#define RenderSignature(UIClass) \
const class UIClass* ui, UIVector posScale, UIVector sizeScale

/**
 * @internal
 * @def DEFINE_RENDER_PASSTHROUGH
 * @brief Defines passthrough for UI rendering in UIElement classes
 * @details Call this macro at the start of widget element declarations that require custom rendering
 * @endinternal
 */
#define DEFINE_RENDER_PASSTHROUGH \
inline virtual void Internal_RenderPassthrough(Internal::UIRenderer* r, UIVector po, UIVector so) override { r->Render(this, po, so); }

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

    protected:
        UIRenderer() = default;
    };

#undef RenderSignature

}