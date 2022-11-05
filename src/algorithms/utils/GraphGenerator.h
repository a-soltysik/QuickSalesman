#pragma once

#include "parsers/TspReader.h"

namespace qs::algo::utils
{

auto generateRandomCompleteGraph(tsplib::Graph::Vertex order) -> tsplib::Graph;

}
