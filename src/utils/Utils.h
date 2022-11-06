#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>

#if defined(NDEBUG)
#include "Parser.h"
#endif

#if defined(_WIN32)

#if !defined(NOMINMAX)
#define NOMINMAX
#endif

#include <windows.h>
#endif

namespace qs::utils
{

// Functions from GoogleBenchmark

#if !defined(_MSC_VER)
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
#else
void useCharPointer(char const volatile*);

template<class Tp>
auto doNotOptimize(Tp& value) -> void
{
    useCharPointer(&reinterpret_cast<char const volatile&>(value));
}
#endif

template<typename T>
std::optional<T> getInput(std::istream& is)
{
#if defined(NDEBUG)
    static_assert(std::is_default_constructible_v<T>);

    auto value = T{};

    if constexpr(std::is_same_v<T, char>)
    {
        auto input = std::string{};
        is >> input;
        value = input[0];
    }
    else if constexpr(std::is_arithmetic_v<T>)
    {
        auto input = std::string{};
        is >> input;
        auto opt = tsplib::utils::stringToNumber<T>(input);
        if (opt.has_value())
        {
            value = opt.value();
        }
        else
        {
            return {};
        }
    }
    else
    {
        is >> value;
    }
    if (!is.fail())
    {
        return value;
    }
#endif
    return {};
}

auto getChoiceFromMenu(const std::string& menu, uint32_t min, uint32_t max) -> uint32_t;

auto getFileContent(const std::filesystem::path& path) -> std::optional<std::string>;

auto setUtf8() -> void;

template<typename T>
auto saturatedAdd(T t1, T t2) -> T
{
    static constexpr auto max = std::numeric_limits<T>::max();
    if (t1 > max - t2)
    {
        return max;
    }
    else
    {
        return t1 + t2;
    }
}

template<typename T>
auto saturatedAdd(std::initializer_list<T> numbers) -> T
{
    auto result = T{};
    for (const auto number : numbers)
    {
        result = saturatedAdd(result, number);
    }
    return result;
}

template<std::integral T>
[[nodiscard]] T getRandom(T from, T to)
{
    static std::random_device device;
    static std::mt19937 rng(device());

    std::uniform_int_distribution<T> distribution(from, to);
    return distribution(rng);
}

}