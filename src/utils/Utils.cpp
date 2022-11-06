#include "Utils.h"

namespace qs::utils
{

auto getChoiceFromMenu(const std::string& menu, uint32_t min, uint32_t max) -> uint32_t
{
    auto wrongChoice = bool{};
    auto choiceValue = std::numeric_limits<uint32_t>::max();

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

auto getFileContent(const std::filesystem::path& path) -> std::optional<std::string>
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

auto setUtf8() -> void
{
#if defined(_WIN32)
    SetConsoleOutputCP(CP_UTF8);
#endif
}

auto useCharPointer(char const volatile*) -> void{}

}