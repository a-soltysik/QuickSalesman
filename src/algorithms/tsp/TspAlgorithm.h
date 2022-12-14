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
    using NeighbourhoodGetter = std::function<std::vector<TspResult::Path>(const TspResult::Path&, size_t)>;
    using BasicSolutionGetter = std::function<TspResult::Path(const tsplib::Graph&)>;
};

[[nodiscard]]
auto getPathLength(std::span<const tsplib::Graph::Vertex> path, const tsplib::Graph& graph) -> TspResult::Distance;

[[nodiscard]]
auto randomRangeReverse(const TspResult::Path& state, size_t numberOfNeighbours) -> std::vector<TspResult::Path>;
[[nodiscard]]
auto randomSwap(const TspResult::Path& state, size_t numberOfNeighbours) -> std::vector<TspResult::Path>;

[[nodiscard]]
auto randomBasicSolution(const tsplib::Graph& graph) -> TspResult::Path;

}
