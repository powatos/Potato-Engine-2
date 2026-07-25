/** @file Timer.hpp */
#pragma once

#include <functional>
#include <tuple>

#include "Core/EventDelegate.hpp"

class TimerManager;

/**
 * @brief Wrapper for Timer that exposes timer properties
 */
struct TimerHandle
{
    friend class TimerManager;

    virtual ~TimerHandle() = default;

    std::string name;
    std::string eventName;

    double duration;
    double elapsed;
    bool hasFinished;
    bool isUpdating;

    TimerHandle() = default;
    TimerHandle(std::string timerName, double duration) : 
        name(std::move(timerName)),
        eventName("___TIMER_" + name),
        duration(duration),
        elapsed(0.0),
        hasFinished(false),
        isUpdating(true)

    {}

    /** @brief Gets name of timer @returns name */
    const std::string& GetName() const { return name; }
    /** @brief Gets duration of timer @returns duration in seconds */
    double GetDuration() const { return duration; }
    /** @brief Gets elapsed time of timer @returns elapsed time in seconds */
    double GetElapsed() const { return elapsed; }
    /** @brief Checks if timer has finished @returns true if finished */
    bool IsFinished() const { return hasFinished; }
    /** @brief Checks if timer is currently updating @returns true if updating */
    bool IsUpdating() const { return isUpdating; }

    virtual void Complete() = 0;

    /**
     * @brief Halts this timer permanently
     * @warning This method is for internal managing use only. Do not manage timers manually. @sa TimerManager
     */
    inline void Halt() { hasFinished = true; }
    
    /**
     * @brief Changes the updating state of the timer
     * @warning This method is for internal managing use only. Do not manage timers manually. @sa TimerManager
     */
    inline void SetUpdating(bool updating) { isUpdating = updating; }

    /**
     * @brief Updates timer for current tick
     * @warning This method is for internal managing use only. Do not manage timers manually. @sa TimerManager
     * @param elapsed_s Time since last update
     */
    inline void UpdateTick(const float elapsed_s) {
        if (hasFinished) { return; }

        elapsed += elapsed_s;
        if (elapsed >= duration) {
            Complete();
        }
    }
};

// TODO: add concepts to enforce strict argument matching to callback args (also in timer manager)

/**
 * @brief Simple timer for scheduling and tracking
 * @warning Do not mutate timers manually. Use the TimerManager interface exposed by the engine instead. @sa TimerManager
 */
template<typename ...CallbackArgs>
struct Timer : public TimerHandle
{
private:
    EventDelegate<CallbackArgs...> delegate;
    std::tuple<CallbackArgs...> args;

public:
    /**
     * @brief Constructs timer object
     * @param timerName Name of timer
     * @warning Do not manage timers manually. @sa TimerManager
     * @param duration Duration of timer in seconds
     */
    template<typename T, typename... Args>
    Timer(std::string timerName, double duration, T* obj, void(T::*callback)(CallbackArgs...), Args&&... args) :
        TimerHandle(timerName, duration),
        delegate(obj, callback),
        args(std::forward<Args>(args)...)
    {
        static_assert(sizeof...(Args) == sizeof...(CallbackArgs));
    }

private:

protected:

    inline virtual void Complete() override {
        if (hasFinished) { return; }
        hasFinished = true;

        std::apply(
            [this](auto&&... unpackedArgs) {
                delegate.Fire(std::forward<decltype(unpackedArgs)>(unpackedArgs)...);
            },
            args
        );
    }

};
