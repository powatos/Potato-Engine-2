/** @file DebugInfo.hpp */
#pragma once

#include "UI/Widget.hpp"

#include "Core/Tickable.hpp"

struct Vector2;

/**
 * @internal
 * @brief Displays debug information on screen updating in real time
 */
class DebugInfo : public Widget, public Tickable
{
    using Widget::Widget;
public:
    void Setup() override;

    ~DebugInfo();

protected:
    virtual void TickPostPhysics(float dt) override;

};