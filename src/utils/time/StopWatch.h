#pragma once

#include <chrono>

namespace qs::utils
{

class StopWatch
{
public:
    auto start() noexcept -> void;
    auto stop() noexcept -> void;

    template<typename DurationType>
    [[nodiscard]]
    auto getTime() const -> DurationType
    {
        return std::chrono::duration_cast<DurationType>(stopPoint - startPoint);
    }

    template<typename DurationType>
    [[nodiscard]]
    auto peak() const -> DurationType
    {
        return std::chrono::duration_cast<DurationType>(std::chrono::high_resolution_clock::now() - startPoint);
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startPoint;
    std::chrono::time_point<std::chrono::high_resolution_clock> stopPoint;
};

}