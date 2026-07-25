/** @file NativeEventBinding.hpp */
#pragma once

#include <string>

#include "Core/EventDelegate.hpp"

/**
 * @brief Wrapper for native event bindings
 * * @tparam CallbackArgs... arguments to pass when notifying the callback function
 */
template<typename ...CallbackArgs>
struct NativeEventBinding
{
// optimized declaration order 
public:
    std::string name;

private:
    EventDelegate<CallbackArgs...> delegate;

public:

    /**
     * @brief Constructs binding with method callback
     * * @tparam T Class of binding object
     * @param name Identifier for binding
     * @param obj Object to bind to
     * @param callback Function to notify
     */
    template<typename T>
    NativeEventBinding(std::string name, T* obj, void(T::*callback)(CallbackArgs...)) : 
        name(name),
        delegate(obj, callback)
    {}

    /**
     * @brief Constructs binding with standalone callback
     * @warning This construction is not recommended. @sa @ref  "TODO<...>(...)"
     * @param name Identifier for binding
     * @param callback Function to notify
     */
    NativeEventBinding(std::string name, void(*callback)(CallbackArgs...)) : 
        name(name),
        delegate(callback)
    {}

    /**
     * @brief Gets EventDelegate reference 
     * @returns Delegate 
     */
    const EventDelegate<CallbackArgs...>& GetDelegate() const { return delegate; }

};