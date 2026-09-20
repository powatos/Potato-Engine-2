/** @file "DebugObjectManager.hpp" */
#pragma once

#include <variant>

#include "Core/EngineSubsystem.hpp"
#include "Util/Color.hpp"
#include "Util/Vector2.hpp"

namespace DebugObjects
{
    struct DebugObject
    {
        Vector2 position;
        float delay;
        Color color;

        [[maybe_unused]] explicit DebugObject(
            const Vector2& position,
            float delay = 2.f,
            const Color& color = Color::RED()
        );
        virtual ~DebugObject() = default;

    };

    struct Point : public DebugObject
    {
        using DebugObject::DebugObject;
    };
    struct Line : public DebugObject
    {
        Vector2 line;
        float thickness;

        [[maybe_unused]] Line(
            const Vector2& position,
            const Vector2& line,
            float delay = 1.f,
            float thickness = 2.f,
            const Color& color = Color::RED()
        );
    };
    struct Box : public DebugObject
    {
        Vector2 size;

        [[maybe_unused]] Box(
            const Vector2& position,
            const Vector2& size,
            float delay = 2.f,
            const Color& color = Color::RED()
        );

    };
    struct Circle : public DebugObject
    {
        float radius;

        [[maybe_unused]] Circle(
            const Vector2& position,
            float radius,
            float delay = 2.f,
            const Color& color = Color::RED()
        );
    };


}

/**
 * @brief Manager for DebugObjects
 * @details DebugObjects are simple objects intended for debugging purposes. Do not use
 * them for gameplay mechanics.
 * @note Debug object rendering/timing is suboptimally performant
 */
struct DebugObjectManager : public EngineSubsystem<DebugObjectManager>
{
    ENGINE_SUBSYSTEM(DebugObjectManager)

    virtual void Resolve() noexcept override;

    template<std::derived_from<DebugObjects::DebugObject> DebugObjectT>
    void Internal_AddObj(DebugObjectT* obj) {
        auto* newObj = new DebugObjectT(*obj);
        objs.push_back( newObj );
        setTimeout(newObj);

    }

    [[nodiscard]] std::vector<DebugObjects::DebugObject*> Internal_GetObjs() const;
    void Internal_DeactivateObject(DebugObjects::DebugObject* obj);

private:
    void setTimeout(DebugObjects::DebugObject* obj);

    std::vector<DebugObjects::DebugObject*> objs;

};
