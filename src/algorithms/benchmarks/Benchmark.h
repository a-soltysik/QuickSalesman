#pragma once

#include "algorithms/Algorithm.h"
#include "utils/Clock.h"

namespace qs::bench
{

template <typename DurationType, typename ResultT, typename... Args>
auto run(algo::Algorithm<ResultT, Args...>&& algorithm,
         const Args&... args) -> std::pair<typename algo::Algorithm<ResultT, Args...>::Result, utils::Clock::Time>
{
    auto clock = utils::Clock{};
    clock.start();
    auto result = algorithm.solve(args...);
    clock.stop();
    return {std::move(result), clock.getTime<DurationType>()};
}

}
