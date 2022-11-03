#pragma once

#include "algorithms/Algorithm.h"
#include "utils/Clock.h"

namespace qs::bench
{

template<typename DurationType, typename ResultT, typename... Args>
auto run(algo::Algorithm<ResultT, Args...>&& algorithm,
         const Args& ... args) -> std::pair<typename algo::Algorithm<ResultT, Args...>::Result, utils::Clock::Time>
{
    auto clock = utils::Clock {};
    clock.start();
    auto result = algorithm.solve(args...);
    clock.stop();
    return {std::move(result), clock.getTime<DurationType>()};
}

template<typename DurationType, typename ResultT, typename... Args>
auto runAverage(size_t times,
                algo::Algorithm<ResultT, Args...>&& algorithm,
                const Args& ... args) -> utils::Clock::Time
{
    auto clock = utils::Clock {};
    clock.start();
    for (auto i = size_t {}; i < times; i++)
    {
        auto result = algorithm.solve(args...);
        utils::doNotOptimize(result);
    }
    clock.stop();
    return clock.template getTime<DurationType>() / times;
}

}
