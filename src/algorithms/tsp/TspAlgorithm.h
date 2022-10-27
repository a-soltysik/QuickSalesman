#pragma once

#include "algorithms/Algorithm.h"

namespace qs::algo::tsp
{

struct Result
{
    std::vector<tsplib::Graph::Vertex> path;
    int64_t length;
};

class TspAlgorithm : public Algorithm<Result>
{

};

[[nodiscard]]
auto getPathLength(const std::vector<tsplib::Graph::Vertex>& path, const tsplib::Graph& graph) -> int64_t;

}
