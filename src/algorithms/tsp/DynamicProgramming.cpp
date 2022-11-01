#include "DynamicProgramming.h"
#include "utils/CombinationsGenerator.h"

#include <algorithm>

namespace qs::algo::tsp
{

auto DynamicProgramming::calculate(const tsplib::Graph& graph) -> std::optional<Result>
{
    if (graph.getOrder() >= std::numeric_limits<uint64_t>::digits)
    {
        return {};
    }

    currentGraph = &graph;
    costs        = makeCostTable();

    return calculate();
}

auto DynamicProgramming::calculate() -> Result
{
    for (auto subsetSize = size_t {2}; subsetSize < currentGraph->getOrder(); subsetSize++)
    {
        auto combinations = utils::CombinationsGenerator {currentGraph->getOrder() - 1, subsetSize};
        while (combinations.hasNext())
        {
            fillCostsForPath(combinationToPath(combinations.getNext()));
        }
    }

    const auto path = getMinimalFinalPath();

    return Result {
        .path = backtracePath(path.predecessor),
        .distance = path.cost,
    };
}

auto DynamicProgramming::backtracePath(tsplib::Graph::Vertex predecessor) -> Result::Path
{
    auto result = backtracePathWithoutEnds(predecessor);

    result.push_back(0);
    result.insert(result.begin(), 0);

    return result;
}

auto DynamicProgramming::makeCostTable() -> std::vector<std::vector<Cost>>
{
    auto result = std::vector(getNumberOfSubsets(currentGraph->getOrder()),
                              std::vector(currentGraph->getOrder(),
                                          Cost {Result::Distance {tsplib::Graph::INFINITY_WEIGHT}, {}}));

    for (auto i = tsplib::Graph::Vertex {1}; i < currentGraph->getOrder(); i++)
    {
        const auto mask = utils::PathMask {}.visit(i).value();
        result[mask][i].cost = currentGraph->getWeightUnchecked({0, i});
    }
    return result;
}

auto DynamicProgramming::getNumberOfSubsets(uint64_t numberOfVertices) -> uint64_t
{
    return 1 << numberOfVertices;
}

auto DynamicProgramming::combinationToPath(utils::PathMask combination) -> utils::PathMask
{
    return {combination.value() << 1};
}

auto DynamicProgramming::findMinimalPathFor(utils::PathMask mask, tsplib::Graph::Vertex end) -> DynamicProgramming::Cost
{
    auto minDistance    = std::numeric_limits<Result::Distance>::max();
    auto minPredecessor = START_POINT;

    for (auto k = START_POINT + 1; k < currentGraph->getOrder(); k++)
    {
        if (end == k)
        {
            continue;
        }

        const auto currentCost = costs[mask.unvisit(end).value()][k];

        if (currentCost.cost != tsplib::Graph::INFINITY_WEIGHT)
        {
            auto currentDistance = currentCost.cost + currentGraph->getWeightUnchecked({k, end});
            if (currentDistance < minDistance)
            {
                minDistance    = currentDistance;
                minPredecessor = k;
            }
        }
    }

    return {minDistance, minPredecessor};
}

auto DynamicProgramming::fillCostsForPath(utils::PathMask mask) -> void
{
    for (auto j = START_POINT + 1; j < currentGraph->getOrder(); j++)
    {
        if (!mask.isVisited(j))
        {
            continue;
        }

        costs[mask.value()][j] = findMinimalPathFor(mask, j);
    }
}

auto DynamicProgramming::getMinimalFinalPath() -> Cost
{
    auto mask = utils::allVisited(currentGraph->getOrder()).unvisit(START_POINT);

    auto cost        = std::numeric_limits<Result::Distance>::max();
    auto predecessor = START_POINT;

    for (auto i = tsplib::Graph::Vertex {1}; i < currentGraph->getOrder(); i++)
    {
        auto currentDistance = costs[mask.value()][i].cost + currentGraph->getWeightUnchecked({i, START_POINT});
        if (cost > currentDistance)
        {
            cost        = currentDistance;
            predecessor = i;
        }
    }

    return {.cost = cost,
            .predecessor = predecessor,
    };
}

auto DynamicProgramming::backtracePathWithoutEnds(tsplib::Graph::Vertex predecessor) -> Result::Path
{
    auto result = Result::Path {};
    result.reserve(currentGraph->getOrder() + 1);

    auto mask = utils::allVisited(currentGraph->getOrder()).unvisit(0);

    for (auto i = size_t {}; i < currentGraph->getOrder() - 1; i++)
    {
        result.push_back(predecessor);
        const auto newPredecessor = costs[mask.value()][predecessor].predecessor;
        mask        = mask.unvisit(predecessor);
        predecessor = newPredecessor;
    }

    std::ranges::reverse(result);

    return result;
}

}

