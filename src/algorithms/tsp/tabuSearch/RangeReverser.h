#pragma once

#include "algorithms/tsp/TspAlgorithm.h"

namespace qs::algo::tsp::ts
{

class RangeReverser
{
public:
    struct NeighbourDescription
    {
        size_t begin;
        size_t end;

        constexpr auto operator==(const NeighbourDescription& rhs) const noexcept -> auto
        {
            return begin == rhs.begin && end == rhs.end;
        }
    };

    RangeReverser(const TspResult& startingSolution, const tsplib::Graph& graph);

    [[nodiscard]]
    auto getNextNeighbour() -> std::pair<NeighbourDescription, TspResult::Distance>;

    [[nodiscard]]
    auto hasNextNeighbour() const -> bool;

    [[nodiscard]]
    auto getNeighbourByDescription(const NeighbourDescription& description) const -> TspResult::Path;

private:
    auto postIncrementState() -> NeighbourDescription;
    auto getWeightBeforeReverse(TspResult::Path::const_iterator begin,
                                TspResult::Path::const_iterator end) const -> tsplib::Graph::Weight;

    auto getWeightAfterReverse(TspResult::Path::const_iterator begin,
                               TspResult::Path::const_iterator end) const-> tsplib::Graph::Weight;

    size_t rangeSize;
    NeighbourDescription currentState;
    const TspResult& startingSolution;
    const tsplib::Graph& graph;
};

}