#pragma once

#include "Algorithm.h"

namespace tsp
{

class BruteForce : Algorithm
{
public:
    auto calculate(const tsplib::Graph& graph) -> std::optional<Result> override;
};

}
