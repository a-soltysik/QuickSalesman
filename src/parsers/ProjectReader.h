#pragma once

#include "TspReader.h"

#ifdef NDEBUG

namespace qs
{

[[nodiscard]]
auto getGraph(std::string_view input) -> std::optional<tsplib::Graph>;

}

#endif