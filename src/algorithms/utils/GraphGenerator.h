#pragma once

#include "parsers/TspReader.h"

namespace qs::algo::utils
{

[[nodiscard]]
auto generateRandomCompleteGraph(tsplib::Graph::Vertex order,
                                 tsplib::Graph::Weight minWeight = 1,
                                 tsplib::Graph::Weight maxWeight = 1000) -> tsplib::Graph;

}
