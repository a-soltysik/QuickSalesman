#pragma once

#include "algorithms/tsp/TspAlgorithm.h"
#include "algorithms/benchmarks/Benchmark.h"
#include "utils/Print.h"

namespace qs
{

class BenchmarkManager
{
public:
    auto menu() -> void;

private:
    static auto printResult(const std::optional<algo::tsp::TspResult>& result) -> void;
    auto readGraphFromFileMenu() -> void;
    auto printGraph() -> void;
    auto generateGraph(size_t order) -> tsplib::Graph;

    template<typename T>
    auto manageAlgorithm(size_t order, T algorithm = T{}) -> void
    {
        auto overallResult = utils::Clock::Time {};
        for (auto i = uint32_t{}; i < 10; i++)
        {
            overallResult += bench::runAverage<std::chrono::milliseconds>(10, algorithm, generateGraph(order));
        }
        utils::print("Średni czas: ", overallResult / 10, "ms");
    }
};

}