/** @file TimerManager.hpp */
#pragma once

#include <vector>
#include <memory>

#include "Core/EngineSubsystem.hpp"
#include "Core/Tickable.hpp"

#include "Timer.hpp"

/**
 * @brief Class that interfaces and handles all timers
 */
class TimerManager : public EngineSubsystem<TimerManager>, public Tickable
{
    ENGINE_SUBSYSTEM(TimerManager)

public:

    /**
     * @brief Creates a new timer
     * @warning The timer returned will go out of scope at the end of the tick it was 
     * added
     * @param duration Duration of the timer
     * @param obj Owning class of callback
     * @param callback Callback function to call at the termination of timer
     * @param args Arguments to pass into the callback
     * @returns An unsafe reference to created timer
     */
    template<typename T, typename... CallbackArgs, typename... Args>
    const TimerHandle& AddTimer(std::string name, double duration, T* obj, void(T::*callback)(CallbackArgs...), Args&&... args);

    /**
     * @brief Halts an ongoing timer
     * @param name Name of timer to halt
     */
    void HaltTimer(const std::string& name);

    /**
     * @brief Pauses an ongoing timer
     * @param name Name of timer to pause
     */
    void PauseTimer(const std::string& name);

    /**
     * @brief Resumes a paused timer
     * @param name Name of timer to resume
     */
    void ResumeTimer(const std::string& name);  

    /**
     * @brief Checks if a timer is active (being managed)
     * @note Timers may be halted but still active
     * @param name Name of timer to check
     * @returns true if timer exists
     */
    bool IsTimerActive(const std::string& name) const;

    /**
     * @brief Gets a timer by name
     * @param name Name of timer to get
     * @warning Getting a timer reference on the same tick it was created is undefined
     * behaviour
     * @warning Ensure timer exists before calling this method. @sa IsTimerActive
     */
    const TimerHandle& GetTimer(const std::string& name);

protected:
    virtual void Tick([[maybe_unused]] float dt) override;

private:
    TimerManager();
    ~TimerManager(); 

    std::vector<std::unique_ptr<TimerHandle>> ActiveTimers;
    std::vector<std::unique_ptr<TimerHandle>> PendingTimers;
};

template<typename T, typename... CallbackArgs, typename... Args>
inline const TimerHandle& TimerManager::AddTimer(std::string name, double duration, T* obj, void(T::*callback)(CallbackArgs...), Args&&... args) {    
    std::unique_ptr<TimerHandle>& timer = PendingTimers.emplace_back( std::make_unique<Timer<CallbackArgs...>>(
        name,
        duration,
        obj,
        callback,
        std::forward<Args>(args)...
    ));

    return *timer;
}