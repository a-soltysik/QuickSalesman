#pragma once

#include "parsers/TspReader.h"

namespace tsp
{

class Algorithm
{
public:
    Algorithm() = default;
    Algorithm(const Algorithm&) = default;
    Algorithm& operator=(const Algorithm&) = default;
    Algorithm(Algorithm&&) = default;
    Algorithm& operator=(Algorithm&&) = default;

    virtual ~Algorithm() = default;

    struct Result
    {
        std::vector<tsplib::Graph::Vertex> path;
        int64_t length;
    };

    virtual auto calculate(const tsplib::Graph& graph) -> std::optional<Result> = 0;
};

[[nodiscard]]
auto getPathLength(const std::vector<tsplib::Graph::Vertex>& path, const tsplib::Graph& graph) -> int64_t;

}

