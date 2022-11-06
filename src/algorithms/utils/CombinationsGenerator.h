#pragma once

#include "PathMask.h"

#include <vector>

namespace qs::algo::utils
{

class CombinationsGenerator
{
public:
    CombinationsGenerator(size_t n, size_t k);

    [[nodiscard]]
    auto getNext() -> PathMask;

    [[nodiscard]]
    auto hasNext() const -> bool;

private:
    std::vector<bool> permutations;
    bool hasNextValue;
};

}
