#include "TspAlgorithm.h"

namespace qs::algo::tsp
{

auto getPathLength(std::span<tsplib::Graph::Vertex> path,
                   const tsplib::Graph& graph) -> TspResult::Distance
{
    auto length = int64_t {};

    for (auto i = size_t {1}; i < path.size(); i++)
    {
        length += graph.getWeightUnchecked({path[i - 1], path[i]});
    }

    return length;
}

}
