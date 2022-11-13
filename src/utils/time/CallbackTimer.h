#include "Timer.h"
#include <condition_variable>
#include <thread>

namespace qs::utils
{

using namespace std::chrono_literals;

template<std::invocable Callback>
class CallbackTimer : public Timer
{
public:
    explicit CallbackTimer(std::chrono::milliseconds time, Callback onTimeout)
        : max {time}
        , onTimeout {onTimeout}
        , wasTimeout {false}
        , running {false} { }

    CallbackTimer(const CallbackTimer&) = delete;
    CallbackTimer(CallbackTimer&&) noexcept = default;
    CallbackTimer& operator=(const CallbackTimer&) = delete;
    CallbackTimer& operator=(CallbackTimer&&) noexcept = default;
    ~CallbackTimer() noexcept override;

    auto start() -> void override;
    auto stop() -> void override;
    auto hasTimeoutOccurred() const -> bool override;
    auto isRunning() const -> bool override;

private:
    mutable std::condition_variable cv;
    mutable std::mutex              mutex;

    std::jthread                    thread;
    const std::chrono::milliseconds max;
    Callback                        onTimeout;
    std::atomic_bool                wasTimeout;
    std::atomic_bool                running;
};

template<std::invocable Callback>
auto CallbackTimer<Callback>::start() -> void
{
    if (running)
    {
        return;
    }

    running    = true;
    wasTimeout = false;

    thread = std::jthread([this]() {
        std::unique_lock<std::mutex> lock(mutex);
        wasTimeout = cv.wait_for(lock, max) == std::cv_status::timeout;
        running    = false;
        if (wasTimeout)
        {
            onTimeout();
        }
    });
}

template<std::invocable Callback>
auto CallbackTimer<Callback>::stop() -> void
{
    cv.notify_one();
}

template<std::invocable Callback>
auto CallbackTimer<Callback>::hasTimeoutOccurred() const -> bool
{
    return wasTimeout;
}

template<std::invocable Callback>
CallbackTimer<Callback>::~CallbackTimer() noexcept
{
    cv.notify_one();
}

template<std::invocable Callback>
auto CallbackTimer<Callback>::isRunning() const -> bool
{
    return running;
}

}
