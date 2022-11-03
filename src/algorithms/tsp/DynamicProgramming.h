#pragma once

#include "TspAlgorithm.h"
#include "utils/PathMask.h"

namespace qs::algo::tsp
{

class DynamicProgramming : public TspAlgorithm
{
public:
    [[nodiscard]]
    auto solve(const tsplib::Graph& graph) -> Result override;

private:
    struct Cost
    {
        TspResult::Distance cost;
        tsplib::Graph::Vertex predecessor;
    };

    [[nodiscard]]
    auto solve() -> ResultValue;

    [[nodiscard]]
    auto backtracePath(tsplib::Graph::Vertex predecessor) -> TspResult::Path;

    [[nodiscard]]
    auto backtracePathWithoutEnds(tsplib::Graph::Vertex predecessor) -> TspResult::Path;

    [[nodiscard]]
    auto findMinimalPathFor(utils::PathMask mask, tsplib::Graph::Vertex end) -> Cost;

    auto fillCostsForPath(utils::PathMask mask) -> void;

    [[nodiscard]]
    auto getMinimalFinalPath() -> Cost;

    [[nodiscard]]
    auto makeCostTable() -> std::vector<std::vector<Cost>>;

    [[nodiscard]]
    static auto getNumberOfSubsets(uint64_t numberOfVertices) -> uint64_t;

    [[nodiscard]]
    static auto combinationToPath(utils::PathMask combination) -> utils::PathMask;

    static constexpr auto START_POINT = tsplib::Graph::Vertex{0};

    std::vector<std::vector<Cost>> costs;
    const tsplib::Graph* currentGraph;
};

}
