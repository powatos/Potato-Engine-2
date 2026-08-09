/** @file EventManager.hpp */
#pragma once

#include <any>
#include <unordered_map>

#include "Core/EngineSubsystem.hpp"
#include "Core/Singleton.hpp"
#include "NativeEventBinding.hpp"

#include "Debug/Log.hpp"

/**
 * @brief Class for event managing
 */
class EventManager : public EngineSubsystem<EventManager>
{
    ENGINE_SUBSYSTEM(EventManager)

public:
    virtual void Resolve() noexcept override;

    /**
     * @brief Registers a custom event that can be called by other functions
     * @warning Ensure all bindings registering to the same ID have the same NativeEventBinding template signature 
     * * @tparam ...Args the callback argument types for the function being binded
     * @param EventID ID of the event registering to
     * @param Binding NativeEventBinding representing the binding for the event
     */
    template<typename ...Args>
    void RegisterNativeEvent(std::string EventID, NativeEventBinding<Args...> Binding);

    /**
     * @brief Fires a custom event based on its ID
     * * @tparam ...Args the callback argument types for the function being binded
     * @param EventID
     * @param args the valued arguments to pass to the @ref NativeEventBinding::Fire "Fire()" call
     */
    template<typename ...Args, typename ...CallbackArgs>
    bool FireNativeEvent(std::string EventID, CallbackArgs... args);

    /**
     * @brief Unregisters an event binding
     * * @tparam ...Args the callback argument types for the binding being unregistered
     * @param EventID ID of event to unregister from
     * @param object Object to unregister all events for
     */
    template<typename ...Args>
    void UnregisterNativeEvent(std::string EventID, void* object);
    /**
     * @brief Unregisters an event binding
     * * @tparam ...Args the callback argument types for the binding being unregistered
     * @param BindingName name of the binding to unregister
     */
    template<typename ...Args>
    void UnregisterNativeEvent(std::string BindingName);

private:
    EventManager();
    ~EventManager();
    
    std::unordered_map< std::string, std::vector<std::any> > Bindings;

};

template<typename ...Args>
void EventManager::RegisterNativeEvent(std::string EventID, NativeEventBinding<Args...> Binding) {
    Bindings[EventID].push_back(std::any(Binding));
}

template<typename ...Args, typename ...CallbackArgs>
bool EventManager::FireNativeEvent(std::string EventID, CallbackArgs... args) {
    using namespace std::string_literals;

    for (const auto& [id, bindingList] : Bindings) {
        if (id == EventID) {

            for (const std::any& binding : bindingList) {

                try {
                    NativeEventBinding<Args...> b = std::any_cast< NativeEventBinding<Args...> >(binding);
                    b.GetDelegate().Fire( std::forward<CallbackArgs>(args)... );
                } catch (const std::bad_any_cast& e) {
                    LOG(LogType::ERROR, "While firing event {}", e.what());
                }
            }

            return true;
        }
    }

    return false;
}

template<typename ...Args>
void EventManager::UnregisterNativeEvent(std::string EventID, void* object) {
    using namespace std::string_literals;

    for (auto& [id, bindingList] : Bindings) {
        if (id != EventID) { continue; }

        for (auto it = bindingList.begin(); it != bindingList.end();) {

            try {
                NativeEventBinding<Args...> b = std::any_cast< NativeEventBinding<Args...> >(*it);
                
                if (b.GetDelegate().GetInstance() == object) { it = bindingList.erase(it); continue; }

            } catch (const std::bad_any_cast& e) {
                LOG(LogType::ERROR, "While unregistering native event by object: {}", e.what());
            }

            ++it;
        }

    }
}

template<typename ...Args>
void EventManager::UnregisterNativeEvent(std::string BindingName) {
    using namespace std::string_literals;

    for (auto& [id, bindingList] : Bindings) {

        for (auto it = bindingList.begin(); it != bindingList.end();) {

            try {
                NativeEventBinding<Args...> b = std::any_cast< NativeEventBinding<Args...> >(*it);
                
                if (b.name == BindingName) { it = bindingList.erase(it); continue; }

            } catch (const std::bad_any_cast& e) {
                LOG(LogType::ERROR, "While unregistering native event by name: {}", e.what());
            }

            ++it;
        }

    }

}