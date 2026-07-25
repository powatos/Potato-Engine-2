/** @file Tickable.hpp */
#pragma once

class TickController;

/**
 * @brief Abstract class that route ticking functionality
 * @note This class \em must be inherited from for object classes that should tick.
 * This class has no inherent functionality.
 * @details `dt` represents the time elapsed since last frame in ms
 * @note Always call base method when overriding: 
 * @code
 * void Class::TickPreInput() {
 *     BaseClass::TickPreInput();
 *     // ...
 * }
 * @endcode
 */
class Tickable 
{
    friend class TickController;

public:
    Tickable(Tickable&&) noexcept = default;
    Tickable& operator =(Tickable&&) noexcept = default;

protected:
    Tickable(); // protected constructor lets ONLY derived classes call it
    virtual ~Tickable() = 0; // protected destructor stops compiler from allocating new, pure virtual makes abstract

    virtual void TickPreInput(float dt) {}
    virtual void _TickInput(float dt) {}
    virtual void Tick(float dt) {}; //< @brief Tick event called on the Update step
    virtual void TickPostUpdate(float dt) {}
    virtual void _TickPhysics(float dt) {}
    virtual void TickPostPhysics(float dt) {}
    virtual void _TickRender(float dt) {}
    virtual void TickPostRender(float dt) {}

};
