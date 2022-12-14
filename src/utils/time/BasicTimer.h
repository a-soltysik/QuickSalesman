#pragma once

#include "Timer.h"
#include "StopWatch.h"

namespace qs::utils
{

class BasicTimer : public Timer
{
public:
    explicit BasicTimer(std::chrono::milliseconds time)
        : max {time}
        , running {false} { }

    auto start() -> void override;
    auto stop() -> void override;

    [[nodiscard]]
    auto hasTimeoutOccurred() const -> bool override;
    [[nodiscard]]
    auto isRunning() const -> bool override;

private:
    const std::chrono::milliseconds max;
    StopWatch                       stopWatch;
    bool                            running;
};

}