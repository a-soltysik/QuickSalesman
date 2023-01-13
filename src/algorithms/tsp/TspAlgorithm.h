#pragma once

#include "algorithms/Algorithm.h"
#include <span>

namespace qs::algo::tsp
{

struct TspResult
{
    using Path = std::vector<tsplib::Graph::Vertex>;
    using Distance = int64_t;

    Path path;
    Distance distance;
};

class TspAlgorithm : public Algorithm<TspResult, tsplib::Graph>
{
public:
    using BasicSolutionGetter = TspResult::Path(&)(const tsplib::Graph&);
};

[[nodiscard]]
auto getPathLength(std::span<const tsplib::Graph::Vertex> path, const tsplib::Graph& graph) -> TspResult::Distance;

[[nodiscard]]
auto makeTspResult(TspResult::Path&& state, const tsplib::Graph& graph) -> TspResult;

[[nodiscard]]
auto randomBasicSolution(const tsplib::Graph& graph) -> TspResult::Path;

}
