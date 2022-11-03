#pragma once

#include <iostream>
#include <ranges>

namespace qs::utils
{

auto print(const auto& t) -> void
{
    std::cout << t << '\n';
}

auto print(const std::ranges::range auto& range) -> void
{
    for (const auto& e : range)
    {
        std::cout << e << ' ';
    }
    std::cout << '\n';
}

auto print(const auto& item, const auto& ... items) -> void
{
    std::cout << item;
    ((std::cout << " " << items), ...) << "\n";
}

// Functions from GoogleBenchmark

template<class Tp>
auto doNotOptimize(Tp const& value) -> void
{
    asm volatile("" : : "r,m"(value) : "memory");
}

template<class Tp>
auto doNotOptimize(Tp& value) -> void
{
#if defined(__clang__)
    asm volatile("" : "+r,m"(value) : : "memory");
#else
    asm volatile("" : "+m,r"(value) : : "memory");
#endif
}

}