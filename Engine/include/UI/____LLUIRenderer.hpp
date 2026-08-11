/** @file ___LLUIRenderer.hpp */
#pragma once

struct UIVector;


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
inline virtual void ___Render_Passthrough(___LLUIRenderer* r, UIVector po, UIVector so) override { r->Render(this, po, so); }

/**
 * @brief Internal low level rendering interface for UI objects
 */
class ___LLUIRenderer
{
public:
    virtual ~___LLUIRenderer() = default;

    virtual void Render(RenderSignature(Widget)) = 0;

    virtual void Render(RenderSignature(TextElement)) = 0;
    virtual void Render(RenderSignature(BoxElement)) = 0;

protected:
    ___LLUIRenderer() = default;
};

#undef RenderSignature
