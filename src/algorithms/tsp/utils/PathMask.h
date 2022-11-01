#pragma once

#include <algorithm>

namespace qs::algo::tsp::utils
{

struct PathMask;

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
        return (mask & (1 << saturate(vertex))) > 0;
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

}