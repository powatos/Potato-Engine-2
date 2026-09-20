/** @file "DebugObjectManager.cpp" */

#include "Debug/DebugObjectManager.hpp"

#include "Util/TimerManager.hpp"

namespace DebugObjects
{
    namespace
    {
        template<std::derived_from<DebugObject> T>
        void add(T* obj) {
            DebugObjectManager::Get()->Internal_AddObj(obj);
        }
    }

    [[maybe_unused]] DebugObject::DebugObject(const Vector2& position, float delay, const Color& color) :
        position(position),
        delay(delay),
        color(color)
    {}

    [[maybe_unused]] Line::Line(const Vector2& position, const Vector2& line, float delay, float thickness, const Color& color) :
        DebugObject(position, delay, color),
        line(line),
        thickness(thickness)
    { add(this); }

    [[maybe_unused]] Box::Box(const Vector2& position, const Vector2& size, float delay, const Color& color) :
        DebugObject(position, delay, color),
        size(size)
    { add(this); }

    [[maybe_unused]] Circle::Circle(const Vector2& position, float radius, float delay, const Color& color) :
        DebugObject(position, delay, color),
        radius(radius)
    { add(this); }


}

std::vector<DebugObjects::DebugObject*> DebugObjectManager::Internal_GetObjs() const {
    return objs;
}

void DebugObjectManager::setTimeout(DebugObjects::DebugObject* obj) {
    TimerManager::Get()->AddTimer(
        "Internal_DebugObject",
        obj->delay,
        this,
        &DebugObjectManager::Internal_DeactivateObject,
        obj
    );
}

void DebugObjectManager::Internal_DeactivateObject(DebugObjects::DebugObject* obj) {

    for (auto it = objs.begin(); it != objs.end(); ++it) {
        if (*it == obj) {
            objs.erase(it);
            return delete obj;
        }
    }

}

void DebugObjectManager::Resolve() noexcept {
    for (const auto* obj : objs) {
        delete obj;
    }
}


