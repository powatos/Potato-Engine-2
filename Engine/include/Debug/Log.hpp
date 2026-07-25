/** @file Debug.hpp */
#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "fmt/format.h"

class Logger;
enum class LogType;

/**
 * @brief Default log object
 * @details This is a Logger object reference. Use as function call: `LOG(...)`
 * @see Logger::operator()
 */
extern Logger LOG;

#define logtypes \
    X(INFO) \
    X(WARNING) \
    X(ERROR) \
    X(VITAL) \
    X(DEBUG)

#define X(name) name,
enum class LogType { logtypes };
#undef X

struct LogEntry
{
    std::string timestamp;
    std::string message;
    LogType type;
};

/**
 * @brief Logging functionality
 * @details Use engine externed Logger objects for logging \n To create a custom logger, see Debug
 */
class Logger
{
public:
    Logger();

    /**
     * @brief Logs message to log file with given type
     * * @tparam Args arguments for formatting
     * @param type type of log message
     * @param message message to log
     * @param args arguments to format into message
     */
    template <typename... Args>
    bool operator ()(const LogType type, fmt::runtime_format_string<> message, Args&&... args);
    template <typename... Args>
    bool operator ()(const LogType type, fmt::format_string<Args...> message, Args&&... args);

    inline static const std::unordered_map<LogType, std::string> LogColorMap = {
        {LogType::INFO, "\x1b[0;37m"},
        {LogType::WARNING, "\x1b[0;33m"},
        {LogType::ERROR, "\x1b[0;31m"},
        {LogType::VITAL, "\x1b[1;35m"},
        {LogType::DEBUG, "\x1b[0;34m"}
    };

    #define X(name) { LogType::name, std::string(#name) },
    inline static const std::unordered_map<LogType, std::string> LogNameMap = {
        logtypes
    };
    #undef X

    void init(const std::string& path);
    ~Logger();

private:
    static const std::string getTimestampUTC();

    void pushLog(LogEntry entry);
    void processLogs();

    struct LogAsyncImpl;
    std::unique_ptr<LogAsyncImpl> i_LogAsyncImpl;
};

template <typename... Args>
bool Logger::operator ()(const LogType type, fmt::runtime_format_string<> message, Args&&... args) {
    const std::string timestamp = getTimestampUTC(); // minimize latency
    const std::string formatted = fmt::format(message, std::forward<Args>(args)...);

    pushLog( LogEntry{
        timestamp,
        formatted,
        type
    } );

    return true;
}

template <typename... Args>
bool Logger::operator ()(const LogType type, fmt::format_string<Args...> message, Args&&... args) {
    const std::string timestamp = getTimestampUTC(); // minimize latency
    const std::string formatted = fmt::format(message, std::forward<Args>(args)...);

    pushLog( LogEntry{
        timestamp,
        formatted,
        type
    } );

    return true;

}