#pragma once

#include "TspManager.h"
#include "algorithms/tsp/TspAlgorithm.h"

#include <chrono>

namespace qs
{

class HeuristicAlgorithmManager : public TspManager
{
public:
    auto menu() -> void override;

private:
    enum class NeighbourhoodGetter
    {
        SWAP,
        SUBRANGE_REVERSE
    };

    auto stopMenu() -> void;
    auto neighbourhoodMenu() -> void;
    auto checkParameters() -> bool;

    std::optional<std::chrono::seconds> time;
    std::optional<NeighbourhoodGetter> getNeighbourhood;
};

}
