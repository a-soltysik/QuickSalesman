#pragma once

#include "parsers/TspReader.h"

namespace qs::algo
{

template<typename Result, typename ...Args>
class Algorithm
{
public:
    Algorithm() = default;
    Algorithm(const Algorithm&) = default;
    Algorithm& operator=(const Algorithm&) = default;
    Algorithm(Algorithm&&) noexcept = default;
    Algorithm& operator=(Algorithm&&) noexcept = default;

    virtual ~Algorithm() noexcept = default;

    virtual auto solve(const Args&... args) -> std::optional<Result> = 0;
};

}

