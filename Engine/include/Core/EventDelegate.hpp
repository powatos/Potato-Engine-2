/** @file EventDelegate.hpp */
#pragma once

#include <functional>

/**
 * @brief A wrapper that carries response delegate information
 * @details Used to subscribe to events in a binding struct/method
 * * @tparam CallbackArgs arguments to pass when notifying the callback function
 */
template<typename... CallbackArgs>
struct EventDelegate
{
private:
    std::function<void(CallbackArgs...)> callback;
    void* instance = nullptr;

public:

    EventDelegate() = default;
    ~EventDelegate() = default;

    /**
     * @anchor event-delegate-template-constructor
     * @brief Constructs delegate with class method binding
     * * @tparam BindingClassO Class of object for binding
     * * @tparam BindingClassC Class of callback to bind
     * @param obj object to bind to
     * @param method callback with additional arguments
     */
    template<typename BindingClassO, typename BindingClassC>
    requires std::is_base_of_v<BindingClassC, BindingClassO>
    EventDelegate(BindingClassO* obj, void(BindingClassC::*method)(CallbackArgs...)) {
        instance = obj;
        callback = [obj, method](CallbackArgs... args) { 
            (obj->*method)(std::forward<CallbackArgs>(args)...); // forward ensures value category preserved
        };
    }

    /**
     * @brief Constructs bare delegate
     * @warning This construction is not recommended. @sa @ref event-delegate-template-constructor "EventDelegate<...>(...)"
     * @param func callback with additional arguments
     */
    EventDelegate(void(*func)(CallbackArgs...)) {
        callback = func;
    }

    /**
     * @brief Fires event and forwards arguments to callback
     * @returns true if the fire was sucessfull
     */
    bool Fire(CallbackArgs... args) const {
        if (callback == nullptr) { return false; }
        callback( std::forward<CallbackArgs>(args)... );
        return true;
    }

    /** @brief Gets binded object @returns `void*` object */
    inline void* GetInstance() const { return instance; }

};
