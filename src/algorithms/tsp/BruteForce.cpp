#include "BruteForce.h"

namespace qs::algo::tsp
{

auto BruteForce::calculate(const tsplib::Graph& graph) -> std::optional<Result>
{
    auto vertices = graph.getVertices();
    if (vertices.size() < 2)
    {
        return {};
    }
    vertices.push_back(vertices.front());

    auto result = Result {
        .path = {},
        .length = std::numeric_limits<decltype(Result::length)>::max()
    };

    const auto first = std::next(vertices.begin());
    const auto last = std::prev(std::prev(vertices.end()));
    do
    {
        const auto length = getPathLength(vertices, graph);

        if (length < result.length)
        {
            result.length = length;
            result.path   = vertices;
        }

    }
    while (std::next_permutation(first, last));

    return result;
}

}
