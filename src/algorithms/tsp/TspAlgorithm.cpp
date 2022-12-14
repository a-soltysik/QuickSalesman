#include "TspAlgorithm.h"
#include "utils/Utils.h"

#include <cmath>

namespace qs::algo::tsp
{

auto getRandomEdge(const TspResult::Path& path) -> tsplib::Graph::Edge
{
    auto edge = tsplib::Graph::Edge{};

    while (edge.first == edge.second)
    {
        edge.first = qs::utils::getRandom<tsplib::Graph::Vertex>(1, path.size() - 2);
        edge.second = qs::utils::getRandom<tsplib::Graph::Vertex>(1, path.size() - 2);
    }

    return edge;
}

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

auto randomRangeReverse(const TspResult::Path& state, size_t numberOfNeighbours) -> std::vector<TspResult::Path>
{
    auto result = std::vector<TspResult::Path>();
    result.reserve(numberOfNeighbours);

    for (auto i = size_t {}; i < numberOfNeighbours; i++)
    {
        auto edge = getRandomEdge(state);

        if (edge.first > edge.second)
        {
            std::swap(edge.first, edge.second);
        }

        auto stateCopy = state;
        std::ranges::reverse(stateCopy.begin() + edge.first, stateCopy.begin() + edge.second);
        result.push_back(stateCopy);
    }
    return result;
}

auto randomSwap(const TspResult::Path& state, size_t numberOfNeighbours) -> std::vector<TspResult::Path>
{
    auto result = std::vector<TspResult::Path>();
    result.reserve(numberOfNeighbours);

    for (auto i = size_t {}; i < numberOfNeighbours; i++)
    {
        const auto edge = getRandomEdge(state);

        auto stateCopy = state;
        std::swap(stateCopy[edge.first], stateCopy[edge.second]);
        result.push_back(std::move(stateCopy));
    }

    return result;
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
