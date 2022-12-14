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
    virtual auto menu() -> void = 0;

protected:
    template<typename T>
    auto manageAlgorithm(T t = T{}) -> void
    {
        if (graph.has_value())
        {
            printResult(qs::bench::run<std::chrono::milliseconds>(std::move(t), graph.value()));
        }
        else
        {
            print("Graf nie został załadowany");
        }
    }

    static auto printResult(const std::pair<algo::tsp::TspAlgorithm::Result, std::chrono::milliseconds>& result) -> void;
    auto readGraphFromFileMenu() -> void;
    auto printGraph() -> void;



    std::optional<tsplib::Graph> graph;
};

}