#include "BasicTimer.h"

namespace qs::utils
{

auto BasicTimer::start() -> void
{
    stopWatch.start();
    running = true;
}

auto BasicTimer::stop() -> void
{
    stopWatch.stop();
    running = false;
}

auto BasicTimer::hasTimeoutOccurred() const -> bool
{
    if (running)
    {
        return !isRunning();
    }
    else
    {
        if (stopWatch.getTime<std::chrono::milliseconds>() > max)
        {
            return true;
        }
        return false;
    }
}

auto BasicTimer::isRunning() const -> bool
{
    if (stopWatch.peak<std::chrono::milliseconds>() > max)
    {
        return false;
    }
    return true;
}

}