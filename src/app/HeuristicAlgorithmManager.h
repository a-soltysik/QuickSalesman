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
    auto stopMenu() -> void;
    auto neighbourhoodMenu() -> void;
    auto checkParameters() -> bool;

    std::optional<std::chrono::seconds> time;
    std::optional<algo::tsp::TspAlgorithm::NeighbourhoodGetter> getNeighbourhood;
};

}
