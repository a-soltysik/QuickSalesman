#pragma once

#include "parsers/TspReader.h"
#include "algorithms/tsp/TspAlgorithm.h"
#include "algorithms/benchmarks/Benchmark.h"
#include "utils/Print.h"

namespace qs
{

class TspManager
{
public:
    auto menu() -> void;

private:
    static auto printResult(const std::pair<algo::tsp::TspAlgorithm::Result, utils::Clock::Time>& result) -> void;
    auto readGraphFromFileMenu() -> void;
    auto printGraph() -> void;

    template<typename T>
    auto manageAlgorithm(T t = T{}) -> void
    {
        if (graph.has_value())
        {
            printResult(qs::bench::run<std::chrono::milliseconds>(std::move(t), graph.value()));
        }
        else
        {
            utils::print("Graf nie został załadowany");
        }
    }

    std::optional<tsplib::Graph> graph;
};

}