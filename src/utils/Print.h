#pragma once

#include <iostream>

namespace qs::utils
{

namespace detail
{

template<typename T>
concept RangeNotString = std::ranges::range<T> && (!std::convertible_to<T, std::string_view>);

auto spaceAndPrint(const auto& t) -> void
{
    std::cout << ' ' << t;
}

auto spaceAndPrint(const RangeNotString auto& range) -> void
{
    std::cout << ' ';
    for (const auto& e : range)
    {
        std::cout << e << ' ';
    }
}

}

auto print(const auto& t) -> void
{
    std::cout << t << '\n';
}

auto print(const detail::RangeNotString auto& range) -> void
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
    (detail::spaceAndPrint(items), ...);
    std::cout << '\n';
}

}