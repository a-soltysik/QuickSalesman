#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>

#include "../ext/TspReader/include/utils/Numbers.h"
#include "../ext/TspReader/include/Graph.h"

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
std::optional<T> getInput(std::istream& is, std::string_view hint = "")
{
#if defined(NDEBUG)
    static_assert(std::is_default_constructible_v<T>);

    std::cout << hint;

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

inline auto getRandomGenerator() -> std::mt19937&
{
    thread_local auto device = std::random_device {};
    thread_local auto rng    = std::mt19937 {device()};
    return rng;
}

template<std::integral T>
[[nodiscard]]
auto getRandom(T from, T to) -> T
{
    if (from > to)
    {
        std::swap(from, to);
    }

    auto distribution = std::uniform_int_distribution<T> {from, to};
    return distribution(getRandomGenerator());
}

template<std::floating_point T>
[[nodiscard]]
auto getRandom(T from, T to) -> T
{
    if (from > to)
    {
        std::swap(from, to);
    }

    auto distribution = std::uniform_real_distribution<T> {from, to};
    return distribution(getRandomGenerator());
}

template<class T, template<class...> class Template>
struct isSpecialization : std::false_type { };

template<template<class...> class Template, class... Args>
struct isSpecialization<Template<Args...>, Template> : std::true_type { };

auto hashCombine(std::size_t& seed);

template <typename T, typename... Other>
auto hashCombine(std::size_t& seed, const T& t, const Other&... other)
{
    std::hash<T> hasher;
    seed ^= hasher(t) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    hashCombine(seed, other...);
}

auto choose(double probability) -> bool;

}

template<typename T>
auto operator+(const std::optional<T>& opt1, const std::optional<T>& opt2) -> std::optional<T>
{
    if (opt1.has_value())
    {
        return opt1.value();
    }
    if (opt2.has_value())
    {
        return opt2.value();
    }
    return {};
}