#pragma once

#include "algorithms/tsp/TspAlgorithm.h"

namespace qs::algo::tsp::ts
{

class VertexSwapper
{
public:
    struct NeighbourDescription
    {
        size_t first;
        size_t second;

        constexpr auto operator==(const NeighbourDescription& rhs) const noexcept -> auto
        {
            return first == rhs.first && second == rhs.second;
        }
    };

    VertexSwapper(const TspResult& startingSolution, const tsplib::Graph& graph);

    [[nodiscard]]
    auto getNextNeighbour() -> std::pair<NeighbourDescription, TspResult::Distance>;

    [[nodiscard]]
    auto hasNextNeighbour() const -> bool;

    [[nodiscard]]
    auto getNeighbourByDescription(const NeighbourDescription& description) const -> TspResult::Path;

private:
    auto postIncrementState() -> NeighbourDescription;

    NeighbourDescription currentState;
    const TspResult& startingSolution;
    const tsplib::Graph& graph;
};

}