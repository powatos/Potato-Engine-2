/** @file TimerManager.cpp */

#include "Debug/Log.hpp"

#include "Util/TimerManager.hpp"

TimerManager::TimerManager() {
    LOG(LogType::VITAL, "TimerManager constructed");

}

void TimerManager::Tick([[maybe_unused]] float dt) {

    if (!PendingTimers.empty()) {
        ActiveTimers.insert(ActiveTimers.end(), std::make_move_iterator(PendingTimers.begin()), std::make_move_iterator(PendingTimers.end()));
        PendingTimers.clear();
    }

    std::vector<std::unique_ptr<TimerHandle>> haltedTimers;

    for (auto it = ActiveTimers.begin(); it != ActiveTimers.end();) {
        auto& timer = *it;

        if (timer->IsUpdating()) {
            timer->UpdateTick(dt);
        }

        if (timer->IsFinished()) {
            timer.reset();
            it = ActiveTimers.erase(it);
        } else {
            ++it;
        }

        
    }
}

void TimerManager::HaltTimer(const std::string &name) {
    for (auto& timer : ActiveTimers) {
        if (timer->GetName() == name) {
            timer->Halt();
        }
    }
    for (auto& timer : PendingTimers) {
        if (timer->GetName() == name) {
            timer->Halt();
        }
    }
}

void TimerManager::PauseTimer(const std::string &name) {
    for (auto& timer : ActiveTimers) {
        if (timer->GetName() == name) {
            timer->SetUpdating(false);
        }
    }
}

void TimerManager::ResumeTimer(const std::string &name) {
    for (auto& timer : ActiveTimers) {
        if (timer->GetName() == name) {
            timer->SetUpdating(true);
        }
    }
}

bool TimerManager::IsTimerActive(const std::string &name) const {
    for (const auto& timer : ActiveTimers) {
        if (timer->GetName() == name) {
            return true;
        }
    }

    return false;
}

const TimerHandle& TimerManager::GetTimer(const std::string &name) {
    for (auto& timer : ActiveTimers) {
        if (timer->GetName() == name) {
            return *timer;
        }
    }

    LOG(LogType::ERROR, "Timer with name '{}' not found", name);
    throw std::runtime_error("Timer not found");
}

TimerManager::~TimerManager() {
}