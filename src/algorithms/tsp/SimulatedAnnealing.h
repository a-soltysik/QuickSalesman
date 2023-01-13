#pragma once

#include "TspAlgorithm.h"

#include <chrono>

namespace qs::algo::tsp
{

class SimulatedAnnealing : public TspAlgorithm
{
public:
    using NeighbourGetter = TspResult::Path(&)(const TspResult::Path&);

    SimulatedAnnealing(NeighbourGetter getNeighbour, BasicSolutionGetter getBasicSolution,
                       std::chrono::seconds time);
    [[nodiscard]]
    auto solve(const tsplib::Graph& graph) -> Result override;

private:
    static auto getProbability(int64_t costDifference, double temperature) -> double;
    auto getInitialTemperature(const tsplib::Graph& graph) -> double;

    NeighbourGetter     getNeighbour;
    BasicSolutionGetter getBasicSolution;
    const std::chrono::seconds time;
};

namespace sa
{

auto randomRangeReverse(const TspResult::Path& state) -> TspResult::Path;
auto randomSwap(const TspResult::Path& state) -> TspResult::Path;

}

}
