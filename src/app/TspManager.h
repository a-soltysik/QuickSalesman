#pragma once

#include "parsers/TspReader.h"
#include "algorithms/tsp/TspAlgorithm.h"

namespace qs
{

class TspManager
{
public:
    auto menu() -> void;

private:
    static auto printResult(const std::optional<algo::tsp::TspResult>& result) -> void;
    auto readGraphFromFileMenu() -> void;
    auto printGraph() -> void;

    template<typename T>
    auto manageAlgorithm(const auto&... args) -> void
    {
        if (graph.has_value())
        {
            printResult(T{args...}.solve(graph.value()));
        }
        else
        {
            utils::print("Graf nie został załadowany");
        }
    }

    std::optional<tsplib::Graph> graph;
};

}