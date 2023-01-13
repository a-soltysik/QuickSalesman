#include "VertexSwapper.h"
#include "utils/Print.h"

#include <utility>
#include <algorithm>

namespace qs::algo::tsp::ts
{

VertexSwapper::VertexSwapper(const TspResult& startingSolution, const tsplib::Graph& graph)
    : currentState {1, 2}
    , startingSolution {startingSolution}
    , graph {graph}
{

}

auto VertexSwapper::getNextNeighbour() -> std::pair<NeighbourDescription, TspResult::Distance>
{
    const auto first = std::ranges::find(startingSolution.path, currentState.first);
    const auto second = std::ranges::find(startingSolution.path, currentState.second);

    if (currentState.second - currentState.first == 1) [[unlikely]]
    {
        const auto weightBeforeSwap = graph.getWeightUnchecked({*std::prev(first), *first}) +
                                      graph.getWeightUnchecked({*first, *second}) +
                                      graph.getWeightUnchecked({*second, *std::next(second)});

        const auto weightAfterSwap = graph.getWeightUnchecked({*std::prev(first), *second}) +
                                     graph.getWeightUnchecked({*second, *first}) +
                                     graph.getWeightUnchecked({*first, *std::next(second)});

        return {postIncrementState(), startingSolution.distance + weightAfterSwap - weightBeforeSwap};
    }
    else
    {
        const auto weightBeforeSwap = graph.getWeightUnchecked({*std::prev(first), *first}) +
                                      graph.getWeightUnchecked({*first, *std::next(first)}) +
                                      graph.getWeightUnchecked({*std::prev(second), *second}) +
                                      graph.getWeightUnchecked({*second, *std::next(second)});

        const auto weightAfterSwap = graph.getWeightUnchecked({*std::prev(first), *second}) +
                                     graph.getWeightUnchecked({*second, *std::next(first)}) +
                                     graph.getWeightUnchecked({*std::prev(second), *first}) +
                                     graph.getWeightUnchecked({*first, *std::next(second)});

        return {postIncrementState(), startingSolution.distance + weightAfterSwap - weightBeforeSwap};
    }
}

auto VertexSwapper::hasNextNeighbour() const -> bool
{
    return currentState.second < startingSolution.path.size() - 1 && currentState.first < startingSolution.path.size() - 1;
}

auto VertexSwapper::postIncrementState() -> NeighbourDescription
{
    if (!hasNextNeighbour())
    {
        return currentState;
    }

    const auto previousState = currentState;

    if (++currentState.second == startingSolution.path.size() - 1)
    {
        currentState.first++;
        currentState.second = currentState.first + 1;
    }
    return previousState;
}

auto VertexSwapper::getNeighbourByDescription(const NeighbourDescription& description) const -> TspResult::Path
{
    auto neighbour = startingSolution;
    std::iter_swap(std::ranges::find(neighbour.path, description.first), std::ranges::find(neighbour.path, description.second));
    return neighbour.path;
}


}