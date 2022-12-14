#pragma once

#include "algorithms/tsp/TspAlgorithm.h"
#include "algorithms/benchmarks/Benchmark.h"
#include "utils/Print.h"
#include "algorithms/utils/GraphGenerator.h"


namespace qs
{

class BenchmarkManager
{
public:
    auto menu() -> void;

private:

    template<typename T>
    auto manageAlgorithm(T algorithm = T {}) -> void
    {
        if (!graphOrder.has_value())
        {
            print("Liczba wierzchołków grafu nie została wybrana");
            return;
        }
        auto overallResult = std::chrono::milliseconds {};

        for (auto i = uint32_t {}; i < 100; i++)
        {
            overallResult += bench::run<std::chrono::milliseconds>(
                T {algorithm},
                algo::utils::generateRandomCompleteGraph(graphOrder.value())
            ).second;
        }
        print("Średni czas: ", (overallResult / 10).count(), "ms");
    }

    std::optional<tsplib::Graph::Vertex> graphOrder;
};

}