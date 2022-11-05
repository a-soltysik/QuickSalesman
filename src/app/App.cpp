#include "App.h"
#include "utils/Print.h"
#include "utils/Utils.h"

#include "TspManager.h"
#include "BenchmarkManager.h"

#include <exception>


namespace qs
{

auto App::run() -> int
{
    try
    {
        utils::setUtf8();
        mainMenu();
        return 0;
    }
    catch (const std::exception& e)
    {
        utils::print(e.what());
    }
    catch (...)
    {
        utils::print("Nieznany błąd\n");
    }
    return -1;
}

auto App::mainMenu() -> void
{
    static constexpr const char* MAIN_MENU =
        "Wybierz tryb:\n"
        "1. Tryb algorytmów\n"
        "2. Tryb benchmarków\n"
        "3. Wyjście\n"
        "> ";

    while (true)
    {
        const auto choice = utils::getChoiceFromMenu(MAIN_MENU, 1, 3);
        switch (choice)
        {
        case 1:
            TspManager{}.menu();
            break;
        case 2:
            BenchmarkManager{}.menu();
            break;
        default:
            return;
        }
    }
}
}