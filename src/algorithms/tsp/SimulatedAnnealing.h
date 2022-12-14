#pragma once

#include "TspAlgorithm.h"

#include <chrono>

namespace qs::algo::tsp
{

class SimulatedAnnealing : public TspAlgorithm
{
public:
    SimulatedAnnealing(NeighbourhoodGetter neighbourhoodGetter, BasicSolutionGetter basicSolutionGetter, std::chrono::seconds time);
    [[nodiscard]]
    auto solve(const tsplib::Graph& graph) -> Result override;

private:
    static auto getCostDifference(const TspResult::Path& state, const TspResult::Path& bestSolution, const tsplib::Graph& graph) -> int64_t;
    static auto getProbability(int64_t costDifference, double temperature) -> double;

    NeighbourhoodGetter  getNeighbours;
    BasicSolutionGetter  getBasicSolution;
    std::chrono::seconds time;
};

}
