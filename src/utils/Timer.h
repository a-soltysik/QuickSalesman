#include <condition_variable>

namespace qs::utils
{

using namespace std::chrono_literals;

constexpr auto emptyCallback = [] { };

template<std::invocable Callback = decltype(emptyCallback)>
class Timer
{
public:
    explicit Timer(std::chrono::milliseconds time, Callback onTimeout = emptyCallback)
        : max {time}
        , onTimeout {onTimeout}
        , wasTimeout {false}
        , isStarted {false}
        , running {false} { }

    Timer(const Timer&) = delete;
    Timer(Timer&&) noexcept = default;
    Timer& operator=(const Timer&) = delete;
    Timer& operator=(Timer&&) noexcept = default;
    ~Timer();

    auto start() -> void;
    auto stop() -> void;
    auto hasTimeoutOccurred() const -> bool;
    auto isRunning() const -> bool;

private:
    auto conditionallyJoin() -> void;

    mutable std::condition_variable cv;
    mutable std::mutex              mutex;


    std::thread                     thread;
    const std::chrono::milliseconds max;
    Callback                        onTimeout;
    std::atomic_bool                wasTimeout;
    std::atomic_bool                isStarted;
    std::atomic_bool                running;
};

template<std::invocable Callback>
auto Timer<Callback>::start() -> void
{
    if (isStarted)
    {
        return;
    }
    conditionallyJoin();

    isStarted  = true;
    running    = true;
    wasTimeout = false;

    thread = std::thread([this]() {
        std::unique_lock<std::mutex> lock(mutex);
        wasTimeout = cv.wait_for(lock, max) == std::cv_status::timeout;
        running    = false;
        isStarted  = false;
        if (wasTimeout)
        {
            onTimeout();
        }
    });
}

template<std::invocable Callback>
auto Timer<Callback>::stop() -> void
{
    if (!running)
    {
        return;
    }

    cv.notify_one();
}

template<std::invocable Callback>
auto Timer<Callback>::hasTimeoutOccurred() const -> bool
{
    return wasTimeout;
}

template<std::invocable Callback>
Timer<Callback>::~Timer()
{
    stop();
    conditionallyJoin();
}

template<std::invocable Callback>
auto Timer<Callback>::isRunning() const -> bool
{
    return running;
}

template<std::invocable Callback>
auto Timer<Callback>::conditionallyJoin() -> void
{
    if (thread.joinable())
    {
        thread.join();
    }
}

}
