#include "GraphGenerator.h"
#include "utils/Utils.h"

namespace qs::algo::utils
{

auto generateRandomCompleteGraph(tsplib::Graph::Vertex order,
                                 tsplib::Graph::Weight minWeight,
                                 tsplib::Graph::Weight maxWeight) -> tsplib::Graph
{
    auto result = tsplib::Graph {order};

    for (auto i = tsplib::Graph::Vertex {}; i < order; i++)
    {
        for (auto j = tsplib::Graph::Vertex {}; j < order; j++)
        {
            if (i != j)
            {
                result.addEdge({{i, j}, qs::utils::getRandom(minWeight, maxWeight)});
            }
        }
    }
    return result;
}

}
