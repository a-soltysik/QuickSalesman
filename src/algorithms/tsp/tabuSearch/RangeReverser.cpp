#include "RangeReverser.h"
#include "utils/Print.h"
#include "utils/Utils.h"

#include <utility>
#include <algorithm>

namespace qs::algo::tsp::ts
{

RangeReverser::RangeReverser(const TspResult& startingSolution, const tsplib::Graph& graph)
    : rangeSize {qs::utils::getRandom<size_t>(1, graph.getOrder() / 5)}
    , currentState {1, rangeSize + 1}
    , startingSolution {startingSolution}
    , graph {graph}
{

}

auto RangeReverser::getNextNeighbour() -> std::pair<NeighbourDescription, TspResult::Distance>
{
    const auto begin = startingSolution.path.begin() + currentState.begin;
    const auto end   = startingSolution.path.begin() + currentState.end;

    const auto weightBeforeReverse = getWeightBeforeReverse(begin, end);
    const auto weightAfterReverse  = getWeightAfterReverse(begin, end);

    return {postIncrementState(), startingSolution.distance + weightAfterReverse - weightBeforeReverse};
}

auto RangeReverser::hasNextNeighbour() const -> bool
{
    return currentState.end < startingSolution.path.size();
}

auto RangeReverser::postIncrementState() -> NeighbourDescription
{
    if (!hasNextNeighbour())
    {
        return currentState;
    }

    const auto previousState = currentState;

    currentState.begin++;
    currentState.end++;

    return previousState;
}

auto RangeReverser::getNeighbourByDescription(const NeighbourDescription& description) const -> TspResult::Path
{
    auto neighbour = startingSolution;
    std::ranges::reverse(neighbour.path.begin() + description.begin,
                         neighbour.path.begin() + description.end);

    return neighbour.path;
}

auto RangeReverser::getWeightBeforeReverse(TspResult::Path::const_iterator begin,
                                           TspResult::Path::const_iterator end) const -> tsplib::Graph::Weight
{
    auto weightBeforeReverse = tsplib::Graph::Weight {};
    weightBeforeReverse += graph.getWeightUnchecked({*std::prev(begin), *begin});

    for (auto i = std::next(begin); i < end; i++)
    {
        weightBeforeReverse += graph.getWeightUnchecked({*std::prev(i), *i});
    }

    return weightBeforeReverse + graph.getWeightUnchecked({*std::prev(end), *end});
}

auto RangeReverser::getWeightAfterReverse(TspResult::Path::const_iterator begin,
                                          TspResult::Path::const_iterator end) const -> tsplib::Graph::Weight
{
    auto weightAfterReverse = tsplib::Graph::Weight {};
    weightAfterReverse += graph.getWeightUnchecked({*std::prev(begin), *std::prev(end)});

    for (auto i = std::next(std::make_reverse_iterator(end)); i < std::make_reverse_iterator(begin); i++)
    {
        weightAfterReverse += graph.getWeightUnchecked({*std::prev(i), *i});
    }

    return weightAfterReverse + graph.getWeightUnchecked({*begin, *end});
}


}