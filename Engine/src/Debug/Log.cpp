/** @file Debug.cpp */

#include <chrono>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <iomanip>
#include <fstream>

#include "fmt/chrono.h"

#include "Debug/Log.hpp"

#include <iostream>

Logger LOG;

struct Logger::LogAsyncImpl
{
    std::ofstream logFile;
    std::queue<LogEntry> logQueue;
    std::mutex queueMutex;
    std::condition_variable cv;
    std::thread t;
    std::atomic<bool> isRunning{true};
};

Logger::Logger() : i_LogAsyncImpl(std::make_unique<LogAsyncImpl>()), showTerminalOutput(true) {}

void Logger::pushLog(LogEntry entry) {
    {
        std::lock_guard<std::mutex> lock(i_LogAsyncImpl->queueMutex);
        i_LogAsyncImpl->logQueue.push(std::move(entry));
    }

    i_LogAsyncImpl->cv.notify_one();
}

void Logger::processLogs() {

    while (true) {
        std::queue<LogEntry> localQueue;

        {
            std::unique_lock<std::mutex> lock(i_LogAsyncImpl->queueMutex);

            i_LogAsyncImpl->cv.wait(lock, [this](){
                return !i_LogAsyncImpl->logQueue.empty() || !i_LogAsyncImpl->isRunning;
            });

            if (!i_LogAsyncImpl->isRunning && i_LogAsyncImpl->logQueue.empty()) { break; }

            localQueue.swap(i_LogAsyncImpl->logQueue);
        }

        while (!localQueue.empty()) {
            const LogEntry& entry = localQueue.front();
            const std::string colorMod = LogColorMap.at(entry.type);
            const std::string logName = LogNameMap.at(entry.type);

            i_LogAsyncImpl->logFile 
                << entry.timestamp << " - "
                << logName << '\t'
                << " | "
                << entry.message << '\n';

            if (showTerminalOutput) {
                std::cout
                    << entry.timestamp << " - "
                    << colorMod << logName << '\t' << "\x1b[0m"
                    << " | "
                    << colorMod << entry.message << "\x1b[0m" << '\n';
            }

            localQueue.pop();
        }

        i_LogAsyncImpl->logFile.flush();
        std::cout << std::flush;

    }

}

void Logger::init(const std::string& path){
    i_LogAsyncImpl->logFile.open(path, std::ios::out | std::ios::trunc);
    i_LogAsyncImpl->logFile.clear();

    i_LogAsyncImpl->t = std::thread(&Logger::processLogs, this);

}

const std::string Logger::getTimestampUTC()  {
    using namespace std::chrono;

    auto now = system_clock::now();
    auto s = time_point_cast<seconds>(now);
    auto ms = duration_cast<milliseconds>( now - s ).count();

    // threadsafe (linux specific)
    std::time_t t = system_clock::to_time_t(now);
    std::tm tm{};

#if defined(_WIN32) || defined(_WIN64)
    gmtime_s(&tm, &t);
#else
    gmtime_r(&t, &tm);
#endif

    std::string timestamp = fmt::format("{:%H:%M:%S}.{:03}", tm, ms);

    return timestamp;
}

Logger::~Logger() {

    i_LogAsyncImpl->isRunning = false;
    i_LogAsyncImpl->cv.notify_one();

    if (i_LogAsyncImpl->t.joinable()) {
        i_LogAsyncImpl->t.join();
    }

    if (i_LogAsyncImpl->logFile.is_open()) {
        i_LogAsyncImpl->logFile.close();
    }
}