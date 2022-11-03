#pragma once

#include "TspReader.h"

namespace qs
{

[[nodiscard]]
auto getGraph(std::string_view input) -> std::optional<tsplib::Graph>;

}