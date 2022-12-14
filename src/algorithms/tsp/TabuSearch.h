#pragma once

#include "TspAlgorithm.h"
#include "algorithms/utils/PathMask.h"

#include <chrono>
#include <functional>
#include <deque>

namespace qs::algo::tsp
{

class TabuSearch : public TspAlgorithm
{
public:
    TabuSearch(NeighbourhoodGetter neighbourhoodGetter, BasicSolutionGetter basicSolutionGetter, size_t neighbourhoodSize, std::chrono::seconds time);
    [[nodiscard]]
    auto solve(const tsplib::Graph& graph) -> Result override;
private:
    static auto fitness(const TspResult::Path& state, const tsplib::Graph& graph) -> TspResult::Distance;

    auto findBestCandidate(const TspResult::Path& state, const std::deque<utils::PathMask>& tabuList, const tsplib::Graph& graph) -> TspResult::Path;
    static auto setBestSolution(TspResult::Path& bestSolution, const TspResult::Path& bestCandidate, const tsplib::Graph& graph) -> void;
    static auto updateTabuList(std::deque<utils::PathMask>& tabuList, const TspResult::Path& bestCandidate, const tsplib::Graph& graph) -> void;

    NeighbourhoodGetter  getNeighbours;
    BasicSolutionGetter  getBasicSolution;
    size_t               neighbourhoodSize;
    std::chrono::seconds time;
};

}
