#include "CombinationsGenerator.h"

#include <algorithm>

namespace qs::algo::utils
{

CombinationsGenerator::CombinationsGenerator(size_t n, size_t k)
    : permutations(n)
    , hasNextValue(true)
{
    std::fill(permutations.begin(), permutations.begin() + k, true);
}

auto CombinationsGenerator::getNext() -> PathMask
{
    auto mask = PathMask{};
    for (auto i = size_t{}; i < permutations.size(); i++)
    {
        if (permutations[i])
        {
            mask = mask.visit(i);
        }
    }

    hasNextValue = std::prev_permutation(permutations.begin(), permutations.end());

    return mask;
}

auto CombinationsGenerator::hasNext() const -> bool
{
    return hasNextValue;
}


}