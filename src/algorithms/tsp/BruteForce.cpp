#include "BruteForce.h"
#include "algorithms/utils/PathMask.h"

namespace qs::algo::tsp
{

auto BruteForce::solve(const tsplib::Graph& graph) -> Result
{
    if (!graph.isComplete())
    {
        return {};
    }

    auto vertices = graph.getVertices();
    vertices.push_back(vertices.front());

    auto result = ResultValue {
        .path = {},
        .distance = std::numeric_limits<TspResult::Distance>::max()
    };

    const auto first = std::next(vertices.begin());
    const auto last = std::prev(vertices.end());
    do
    {
        const auto distance = getPathLength(vertices, graph);

        if (distance < result.distance)
        {
            result.distance = distance;
            result.path     = vertices;
        }

    }
    while (std::next_permutation(first, last));

    return result;
}

}
