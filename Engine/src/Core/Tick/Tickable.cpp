/** @file Tickable.cpp */

#include "Core/EventManager.hpp"
#include "Core/TickManager.hpp"
#include "Core/PotatoEngine.hpp"

#include "Core/Tickable.hpp"

Tickable::Tickable() {
    TickManager* controller = TickManager::Get();
    controller->Register(this);
}

Tickable::~Tickable() {
    TickManager* controller = TickManager::Get();
    controller->Unregister(this);
}