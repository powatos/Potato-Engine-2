/** @file Tickable.cpp */

#include "Core/EventController.hpp"
#include "Core/TickController.hpp"
#include "Core/PotatoEngine.hpp"

#include "Core/Tickable.hpp"

Tickable::Tickable() {
    TickController* controller = TickController::Get();
    controller->Register(this);
}

Tickable::~Tickable() {
    TickController* controller = TickController::Get();
    controller->Unregister(this);
}