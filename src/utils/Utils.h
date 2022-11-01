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

auto print(const auto& item, const auto&... items) -> void
{
    std::cout << item;
    ((std::cout << " " << items), ...) << "\n";
}

}