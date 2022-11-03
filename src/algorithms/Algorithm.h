#pragma once

#include "parsers/TspReader.h"

namespace qs::algo
{

template<typename ResultT, typename ...Args>
class Algorithm
{
public:
    using Result = std::optional<ResultT>;
    using ResultValue = typename Result::value_type;

    Algorithm() = default;
    Algorithm(const Algorithm&) = default;
    Algorithm& operator=(const Algorithm&) = default;
    Algorithm(Algorithm&&) noexcept = default;
    Algorithm& operator=(Algorithm&&) noexcept = default;

    virtual ~Algorithm() noexcept = default;

    virtual auto solve(const Args&... args) -> Result = 0;
};

}

