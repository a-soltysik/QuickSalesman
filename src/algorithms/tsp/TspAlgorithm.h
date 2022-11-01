#pragma once

#include "algorithms/Algorithm.h"
#include "utils/Utils.h"
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

class TspAlgorithm : public Algorithm<Result>
{

};

[[nodiscard]]
inline auto getPathLength(std::span<tsplib::Graph::Vertex> path, const tsplib::Graph& graph) -> Result::Distance
{
    auto length = Result::Distance {};

    for (auto i = size_t {1}; i < path.size(); i++)
    {
        length += graph.getWeightUnchecked({path[i  - 1], path[i]});
    }

    return length;
}

}
