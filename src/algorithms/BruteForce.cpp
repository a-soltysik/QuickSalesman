#include "BruteForce.h"

#include <algorithm>

namespace tsp
{

auto BruteForce::calculate(const tsplib::Graph& graph) -> std::optional<Result>
{
    auto vertices = graph.getVertices();
    if (vertices.empty())
    {
        return {};
    }

    auto result = Result{};
    do
    {

    } while (std::ranges::next_permutation(vertices).found);
    return {};
}

}
