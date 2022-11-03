#pragma once

#include "TspReader.h"

namespace qs
{

[[nodiscard]]
auto getGraph([[maybe_unused]] std::string_view input) -> std::optional<tsplib::Graph>;

}