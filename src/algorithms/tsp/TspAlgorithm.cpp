#include "TspAlgorithm.h"
#include "utils/Utils.h"

#include <cmath>

namespace qs::algo::tsp
{

auto getPathLength(std::span<const tsplib::Graph::Vertex> path,
                   const tsplib::Graph& graph) -> TspResult::Distance
{
    auto length = int64_t {};

    for (auto i = size_t {1}; i < path.size(); i++)
    {
        length += graph.getWeightUnchecked({path[i - 1], path[i]});
    }

    return length;
}

auto makeTspResult(TspResult::Path&& state, const tsplib::Graph& graph) -> TspResult
{
    const auto distance = getPathLength(state, graph);
    return {
        std::move(state),
        distance
    };
}

auto randomBasicSolution(const tsplib::Graph& graph) -> TspResult::Path
{
    auto vertices = graph.getVertices();
    vertices.push_back(vertices.front());

    const auto first = std::next(vertices.begin());
    const auto last  = std::prev(vertices.end());
    std::shuffle(first, last, qs::utils::getRandomGenerator());
    return vertices;
}

}
