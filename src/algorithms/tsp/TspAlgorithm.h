#pragma once

#include "algorithms/Algorithm.h"
#include "algorithms/tsp/utils/PathMask.h"
#include <span>

namespace qs::algo::tsp
{

struct Result
{
    using Path = std::vector<tsplib::Graph::Vertex>;
    using Distance = int64_t;

    Path path;
    Distance distance;
};

class TspAlgorithm : public Algorithm<Result, tsplib::Graph>
{

};

[[nodiscard]]
auto getPathLength(std::span<tsplib::Graph::Vertex> path, const tsplib::Graph& graph) -> Result::Distance;

}
