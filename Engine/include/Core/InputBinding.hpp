/** @file InputBinding.hpp */
#pragma once

#include "Core/EventManager.hpp"
#include "Core/Input.hpp"

/**
 * @brief Wrapper for input bindings
 */
struct InputBinding
{
// optimized declaration order 
public:
    /**
     * @brief Identifier for binding
     */
    std::string name;

private:
    EventDelegate<> delegate;

public:
    /// @brief Keycode to bind to
    Keycode key;
    /// @brief InputType to bind to
    InputType type;

    /**
     * @brief Constructs binding with method callback
     * * @tparam To Class of binding object
     * * @tparam Tc Class of binded callback
     * @param key Keycode to bind to
     * @param type InputType to bind to
     * @param name Identifier for binding
     * @param obj Object to bind to
     * @param callback Function to notify
     */
    template<typename To, typename Tc>
    requires std::is_base_of_v<Tc, To>
    InputBinding(Keycode key, InputType type, const std::string& name, To* obj, void(Tc::*callback)()) :
        name(name),
        delegate(obj, callback),
        key(key),
        type(type)
    {}

    /**
     * @brief Constructs binding with standalone callback
     * @note Only use for bindings that will never be unregistered. Otherwise, prefer object-binding @sa InputBinding(Keycode, InputType, std::string, T*, void(T::*)())
     * @param key Keycode to bind to
     * @param type InputType to bind to
     * @param name Identifier for binding
     * @param callback Function to notify
     */
    InputBinding(Keycode key, InputType type, const std::string& name, void(*callback)()) :
        name(name),
        delegate(callback),
        key(key),
        type(type)
    {}

    /** @brief Gets EventDelegate reference @returns Delegate */
    inline const EventDelegate<>& GetDelegate() const { return delegate; }

};