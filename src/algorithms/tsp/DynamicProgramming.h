#pragma once

#include "TspAlgorithm.h"
#include "Utils.h"
#include <span>

namespace qs::algo::tsp
{

class DynamicProgramming : public TspAlgorithm
{
public:
    [[nodiscard]]
    auto calculate(const tsplib::Graph& graph) -> std::optional<Result> override;
private:
    [[nodiscard]]
    auto calculate(utils::PathMask mask, tsplib::Graph::Vertex begin) -> Result::Distance;
    auto getResultForMask(utils::PathMask mask, tsplib::Graph::Vertex begin) -> Result::Distance;
    auto getPath() -> Result::Path;
    static auto subsetToMask(std::span<tsplib::Graph::Vertex> subset) -> utils::PathMask;
    auto makeCostTable() -> std::vector<std::vector<Result::Distance>>;
    static auto getNumberOfSubsets(uint64_t numberOfVertices) -> uint64_t;

    std::vector<std::vector<Result::Distance>> cost;
    const tsplib::Graph* currentGraph;
    tsplib::Graph::Vertex start;
};

}
