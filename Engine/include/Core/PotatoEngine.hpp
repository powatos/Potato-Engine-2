/** @file PotatoEngine.hpp */
#pragma once

#include <memory>
#include <vector>
#include <string>

class IInputController;
class IWindowController;
class EventManager;
class IEngineSubsystem;

namespace Internal
{
/**
 * @def PROJECT(name)
 * @brief Called at the top of the project entrypoint to define the application name
 * @remark Call this macro before the entrypoint function: `int main()`
 * @param name application name
 */
#define PROJECT(name) \
struct ProjectInit { \
    ProjectInit() { \
        Internal::EngineGlobals::appName = name; \
    } \
} Project_Init;

}
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
    static IInputController* GetInputController();
    /** @brief Gets @ref IWindowController "window controller" @returns @ref IWindowController "window controller" */
    static IWindowController* GetWindowController();

protected:
    std::vector<IEngineSubsystem*> SubsystemStack;

    inline static IInputController* InputController = nullptr;
    inline static IWindowController* WindowController = nullptr;


private:
    std::string logPath;
};

namespace Internal::EngineGlobals {
    inline std::string orgName = "PotatoEngine";
    inline std::string appName;
}
