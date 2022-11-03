#pragma once

#include <chrono>

namespace qs::utils
{

class Clock
{
public:
    auto start() noexcept -> void ;
    auto stop() noexcept -> void;


    [[nodiscard]]
    auto getTimeInNanos() const -> int64_t;

    [[nodiscard]]
    auto getTimeInMicros() const -> int64_t;

    [[nodiscard]]
    auto getTimeInMillis() const -> int64_t;

    [[nodiscard]]
    auto getTimeInSeconds() const -> int64_t;

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startPoint;
    std::chrono::time_point<std::chrono::high_resolution_clock> stopPoint;
};

}