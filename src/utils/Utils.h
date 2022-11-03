#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>

#if defined(NDEBUG)
#include "Parser.h"
#endif

#if defined(_WIN32)
#include <windows.h>
#endif

namespace qs::utils
{

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

inline auto getChoiceFromMenu(const std::string& menu, uint32_t min, uint32_t max) -> uint32_t
{
    auto wrongChoice = bool{};
    auto choiceValue = uint32_t {};

    do
    {
        std::cout << menu;
        const auto choice = getInput<uint32_t>(std::cin);
        wrongChoice = !choice.has_value() || (choice.has_value() && (choice.value() < min || choice.value() > max));
        if (wrongChoice)
        {
            std::cout << "Niepoprawny wybór!\n";
        }
        else
        {
            choiceValue = choice.value();
        }
    }
    while (wrongChoice);

    return choiceValue;
}

inline auto getFileContent(const std::filesystem::path& path) -> std::optional<std::string>
{
    auto fin = std::ifstream{path};

    if (!fin.good())
    {
        return {};
    }

    fin.seekg(0, std::ios::end);
    const auto size = fin.tellg();
    auto buffer = std::string(static_cast<size_t>(size), 0);
    fin.seekg(0);
    fin.read(buffer.data(), size);

    return buffer;
}

inline auto setUtf8() -> void
{
#if defined(_WIN32)
    SetConsoleOutputCP(CP_UTF8);
#endif
}

}