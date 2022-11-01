#include "DynamicProgramming.h"

#include <iostream>

#include <span>
#include <limits>
#include <algorithm>

namespace qs::algo::tsp
{

auto DynamicProgramming::calculate(const tsplib::Graph& graph) -> std::optional<Result>
{
    if (graph.getOrder() >= std::numeric_limits<uint64_t>::digits)
    {
        return {};
    }

    auto vertices = graph.getVertices();

    currentGraph = &graph;
    start = vertices.front();
    cost = makeCostTable();

    auto length = calculate(utils::PathMask{}.visit(start), start);
    auto path = getPath();

    for (const auto& e : cost)
    {
        qs::utils::print(e);
    }

    return Result{
        .path = std::move(path),
        .distance = length,
    };
}

auto DynamicProgramming::calculate(utils::PathMask mask, tsplib::Graph::Vertex begin) -> Result::Distance
{
    if (mask == utils::allVisited(currentGraph->getOrder()))
    {
        return (*currentGraph).getWeightUnchecked({start, begin});
    }
    if (cost[begin][mask.value()] != tsplib::Graph::INFINITY_WEIGHT)
    {
        return cost[begin][mask.value()];
    }

    const auto result = getResultForMask(mask, begin);
    cost[begin][mask.value()] = result;
    return result;
}

auto DynamicProgramming::getResultForMask(utils::PathMask mask, tsplib::Graph::Vertex begin) -> Result::Distance
{
    auto result = Result::Distance {tsplib::Graph::INFINITY_WEIGHT};

    for (auto i = tsplib::Graph::Vertex{}; i < currentGraph->getOrder(); i++)
    {
        if (!mask.isVisited(i))
        {
            auto subResult = currentGraph->getWeightUnchecked({i, begin}) + calculate(mask.visit(i), i);
            result = std::min(result, subResult);
        }
    }

    return result;
}

auto DynamicProgramming::getPath() -> Result::Path
{
    auto result = Result::Path {};
    auto mask = utils::allVisited(currentGraph->getOrder());
    result.reserve(currentGraph->getOrder() + 1);

    auto lastIndex = start;
    result.push_back(start);
    for (auto i = tsplib::Graph::Vertex{}; i < currentGraph->getOrder(); i++)
    {
        if (i == start)
        {
            continue;
        }
        auto index = tsplib::Graph::Vertex{};
        for (auto j = tsplib::Graph::Vertex{}; j < currentGraph->getOrder(); j++)
        {
            if (!mask.isVisited(j))
            {
                continue;
            }
            const auto prevDist = cost[index][mask.value()] + currentGraph->getWeightUnchecked({index, lastIndex});
            const auto newDist = cost[j][mask.value()] + currentGraph->getWeightUnchecked({j, lastIndex});
            if (newDist < prevDist)
            {
                index = j;
            }
        }
        result.push_back(index);
        mask = mask.unvisit(index);
        lastIndex = index;
    }
    result.push_back(start);
    std::ranges::reverse(result);

    return result;
}

auto DynamicProgramming::subsetToMask(std::span<tsplib::Graph::Vertex> subset) -> utils::PathMask
{
    auto result = utils::PathMask{};
    for (auto vertex : subset)
    {
        result = result.visit(vertex);
    }

    return {result};
}

auto DynamicProgramming::makeCostTable() -> std::vector<std::vector<Result::Distance>>
{
    return {getNumberOfSubsets(currentGraph->getOrder()),
                               std::vector(currentGraph->getOrder(),
                                           Result::Distance {tsplib::Graph::INFINITY_WEIGHT})};
}

auto DynamicProgramming::getNumberOfSubsets(uint64_t numberOfVertices) -> uint64_t
{
    return 1 << numberOfVertices;
}

}

