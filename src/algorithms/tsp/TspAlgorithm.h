#pragma once

#include "algorithms/Algorithm.h"
#include "algorithms/utils/PathMask.h"
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

};

[[nodiscard]]
auto getPathLength(std::span<tsplib::Graph::Vertex> path, const tsplib::Graph& graph) -> TspResult::Distance;

}
