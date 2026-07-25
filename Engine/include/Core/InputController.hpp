/** @file InputController.hpp */
#pragma once

#include "InputBinding.hpp"

/**
 * @brief Interface to manage input binding operations
 */
class IInputController
{
public:
    virtual ~IInputController() = default;

    /**
     * @brief Registers a single binding
     * @param binding Binding to register to
     */
    virtual void RegisterInputBinding(InputBinding binding) = 0;
    
    /**
     * @brief Registers list of bindings
     * @details Prefer over RegisterInputBinding(InputBinding) for mass bindings
     * @param bindings List of bindings to register
     */
    virtual void RegisterInputBinding(std::initializer_list<InputBinding> bindings) = 0;
    
    /**
     * @brief Unregisters a single binding
     * @param name Identifier for binding to unregister
     */
    virtual void UnregisterInputBinding(std::string name) = 0;
    
    /**
     * @brief Unregisters all bindings tied to an object
     * @param object Object to unregister all bindings for
     */
    virtual void UnregisterAllInputBindings(void* object) = 0;
};