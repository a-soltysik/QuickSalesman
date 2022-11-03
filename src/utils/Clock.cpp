#include "Clock.h"

namespace qs::utils
{

auto Clock::start() noexcept -> void
{
    startPoint = std::chrono::high_resolution_clock::now();
}

auto Clock::stop() noexcept -> void
{
    stopPoint = std::chrono::high_resolution_clock::now();
}

auto Clock::getTimeInNanos() const -> int64_t
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(stopPoint - startPoint).count();
}

auto Clock::getTimeInMicros() const -> int64_t
{
    return std::chrono::duration_cast<std::chrono::microseconds>(stopPoint - startPoint).count();
}

auto Clock::getTimeInMillis() const -> int64_t
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(stopPoint - startPoint).count();
}

auto Clock::getTimeInSeconds() const -> int64_t
{
    return std::chrono::duration_cast<std::chrono::seconds>(stopPoint - startPoint).count();
}

}