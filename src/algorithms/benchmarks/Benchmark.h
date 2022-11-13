#pragma once

#include "algorithms/Algorithm.h"
#include "utils/time/StopWatch.h"
#include "utils/Utils.h"

namespace qs::bench
{

template<typename DurationType, typename ResultT, typename... Args>
auto run(algo::Algorithm<ResultT, Args...>&& algorithm,
         const Args& ... args) -> std::pair<typename algo::Algorithm<ResultT, Args...>::Result, DurationType>
{
    auto clock = utils::StopWatch {};
    clock.start();
    auto result = algorithm.solve(args...);
    clock.stop();
    return {std::move(result), clock.getTime<DurationType>()};
}

template<typename DurationType, typename ResultT, typename... Args>
auto runAverage(size_t times,
                algo::Algorithm<ResultT, Args...>&& algorithm,
                const Args& ... args) -> DurationType
{
    auto clock = utils::StopWatch {};
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
