#pragma once

#include "TspAlgorithm.h"

namespace qs::algo::tsp
{

class BruteForce : public TspAlgorithm
{
public:
    [[nodiscard]]
    auto calculate(const tsplib::Graph& graph) -> std::optional<Result> override;
};

}
