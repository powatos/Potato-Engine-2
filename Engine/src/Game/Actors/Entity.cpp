/** @file Entity.cpp */

#include <algorithm>

#include "Game/Entity.hpp"

Entity::Entity() {
    MaxHealth = 100.f;
    Health = MaxHealth;
}

Entity::~Entity() {

}

float Entity::TakeDamage(float damage) {
    Health = std::clamp(Health - damage, 0.f, MaxHealth);
    return Health;
}

float Entity::AddHealth(float amount) {
    Health = std::clamp(Health + amount, 0.f, MaxHealth);
    return Health;
}

float Entity::GetHealth() const {
    return Health;
}