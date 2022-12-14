#pragma once

#include "algorithms/tsp/TspAlgorithm.h"

#include <algorithm>
#include <limits>
#include <numeric>

namespace qs::algo::utils
{

[[nodiscard]]
constexpr auto saturate(uint64_t vertex) noexcept -> uint64_t
{
    return std::min(vertex, uint64_t {std::numeric_limits<uint64_t>::digits});
}

struct PathMask
{
    uint64_t mask;

    constexpr auto operator<=>(const PathMask& mask) const noexcept = default;

    [[nodiscard]]
    constexpr auto value() const noexcept -> uint64_t
    {
        return mask;
    }

    [[nodiscard]]
    constexpr auto isVisited(uint64_t vertex) const noexcept -> bool
    {
        return (mask & (uint64_t {1} << saturate(vertex))) > 0;
    }

    [[nodiscard]]
    constexpr auto visit(uint64_t vertex) const noexcept -> PathMask
    {
        return {mask | (uint64_t {1} << saturate(vertex))};
    }

    [[nodiscard]]
    constexpr auto unvisit(uint64_t vertex) const noexcept -> PathMask
    {
        return {mask ^ (uint64_t {1} << saturate(vertex))};
    }
};

[[nodiscard]]
constexpr auto allVisited(uint64_t numberOfVertices) noexcept -> PathMask
{
    return {(uint64_t {1} << saturate(numberOfVertices)) - 1};
}

[[nodiscard]]
inline auto toMask(const qs::algo::tsp::TspResult::Path& path) -> PathMask
{
    return std::accumulate(path.cbegin(), path.cend(), PathMask{}, [](auto result, auto vertex) {
        return result.visit(vertex);
    });
}

}

namespace std {

template <>
struct hash<qs::algo::utils::PathMask>
{
    auto operator()(qs::algo::utils::PathMask mask) const -> size_t
    {
        return std::hash<decltype(mask.value())>{}(mask.value());
    }
};

}