#include "StopWatch.h"

namespace qs::utils
{

auto StopWatch::start() noexcept -> void
{
    startPoint = std::chrono::high_resolution_clock::now();
}

auto StopWatch::stop() noexcept -> void
{
    stopPoint = std::chrono::high_resolution_clock::now();
}

}