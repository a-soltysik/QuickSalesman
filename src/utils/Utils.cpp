#include "Utils.h"
#include "Print.h"

namespace qs::utils
{

auto getChoiceFromMenu(const std::string& menu, uint32_t min, uint32_t max) -> uint32_t
{
    auto wrongChoice = bool{};
    auto choiceValue = std::numeric_limits<uint32_t>::max();

    do
    {
        const auto choice = getInput<uint32_t>(std::cin, menu);
        wrongChoice = !choice.has_value() || (choice.has_value() && (choice.value() < min || choice.value() > max));
        if (wrongChoice)
        {
            print("Niepoprawny wybór!");
        }
        else
        {
            choiceValue = choice.value();
        }
    }
    while (wrongChoice);

    return choiceValue;
}

auto getFileContent(const std::filesystem::path& path) -> std::optional<std::string>
{
    auto fin = std::ifstream{path};

    if (!fin.good())
    {
        return {};
    }

    return (std::stringstream{} << fin.rdbuf()).str();
}

auto setUtf8() -> void
{
#if defined(_WIN32)
    SetConsoleOutputCP(CP_UTF8);
#endif
}

auto useCharPointer(char const volatile*) -> void{}

auto choose(double probability) -> bool
{
    return getRandom<double>(0., 1.) < probability;
}

auto hashCombine([[maybe_unused]] std::size_t& seed) {}

}