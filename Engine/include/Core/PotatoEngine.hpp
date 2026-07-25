/** @file PotatoEngine.hpp */
#pragma once

#include <memory>
#include <vector>
#include <string>

class IInputController;
class IHUDController;
class EventController;
class IEngineSubsystem;

/**
 * @def PROJECT(name)
 * @brief Called at the top of the project entrypoint to define the application name
 * @remark Call this macro before the entrypoint function: `int main()`
 * @param name application name
 */
#define PROJECT(name) \
struct ___ProjectInit { \
    ___ProjectInit() { \
        ___ENGINE_GLOBALS::appName = name; \
    } \
} ___ProjectInit;

/**
 * @brief Global engine singleton class
 * @details This class manages the engine and engine subsystems. This class must be configured
 * before playing the game.
 */
class PotatoEngine // implementation in 'Internal/Private/PotatoEngine.cpp'
{
    PotatoEngine();
    ~PotatoEngine();
public:
    /**
     * @brief Global access to engine object
     * @note This class has an uppercased get method
     * @returns Engine reference
     */
    static PotatoEngine& Get();

    /**
     * @brief Loads all subclasses and calls corresponding initializing methods
     * @warning Must be called before BeginPlay()
     */
    void LoadSubclasses();

    /**
     * @brief Starts gameplay
     * @note Must be called to start gameplay
     * @param autoResolve If engine should automatically resolve after a delay (for debugging purposes)
     */
    void BeginPlay(bool autoResolve = false);

    /**
     * @brief Resolve all subclasses and perform resolving engine functionality
     * @warning Must be called before exiting program
     */
    void Resolve() noexcept;

    /** @brief Gets @ref IInputController "input controller" @returns @ref IInputController "input controller" */
    IInputController* GetInputController() const;
    /** @brief Gets @ref ITickController "tick controller" @returns @ref ITickController "tick controller" */
    // ITickController* GetTickController() const;
    /** @brief Gets @ref IHUDController "HUD controller" @returns @ref IHUDController "HUD controller" */
    IHUDController* GetHUDController() const;
    /** @brief Gets @ref EventController "Native event controller" @returns @ref EventController "Native event controller" */
    EventController* GetNativeEventController() const;

protected:
    std::vector<IEngineSubsystem*> SubsystemStack;

    IInputController* InputController{};
    IHUDController* HUDController{};
    EventController* NativeEventController{};


private:
    std::string logPath;
};

namespace ___ENGINE_GLOBALS {
    inline std::string orgName = "PotatoEngine";
    inline std::string appName;
}