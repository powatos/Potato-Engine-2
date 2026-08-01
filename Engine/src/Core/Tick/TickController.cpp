/** @file TickController.cpp */

#include <algorithm>

#include "Core/TickController.hpp"

#include "Core/Tickable.hpp"
#include "Debug/Log.hpp"

TickController::TickController() {
    LOG(LogType::VITAL, "TickController constructed");
}

void TickController::Fire(float dt, TickGroup group) {

    // add from queue
    tickables.reserve(tickables.size() + tickablesQueue.size());
    tickables.insert(
        tickables.end(),
        std::make_move_iterator(tickablesQueue.begin()),
        std::make_move_iterator(tickablesQueue.end())
    );
    tickablesQueue.clear();

    for (auto it = tickables.begin(); it != tickables.end();) {
        if (*it == nullptr) { it = tickables.erase(it); continue; }
        checkObjByGroup(dt, group, *it);
        ++it;   
    }

}

constexpr void TickController::checkObjByGroup(float dt, TickGroup group, Tickable* obj) {
    if (obj == nullptr) { return; }

    switch (group) {
        case TickGroup::PreInput:
            obj->TickPreInput(dt);
            break;
        case TickGroup::_Input:
            obj->_TickInput(dt);
            break;
        case TickGroup::Update:
            obj->Tick(dt);
            break;
        case TickGroup::PostUpdate:
            obj->TickPostUpdate(dt);
            break;
        case TickGroup::_Physics:
            obj->_TickPhysics(dt);
            break;
        case TickGroup::PostPhysics:
            obj->TickPostPhysics(dt);
            break;
        case TickGroup::_Render:
            obj->_TickRender(dt);
            break;
        case TickGroup::PostRender:
            obj->TickPostRender(dt);
            break;
    }
}

void TickController::Register(Tickable* tickable) {
    tickablesQueue.push_back(tickable);
}
void TickController::Unregister(Tickable* tickable) {
    if (auto it = std::find(tickables.begin(), tickables.end(), tickable); it != tickables.end()) {
        *it = nullptr;
    }
}

void TickController::Resolve() noexcept {
    LOG(LogType::VITAL, "Resolving TickController");
}

TickController::~TickController() {
}