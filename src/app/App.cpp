#include "App.h"
#include "utils/Print.h"
#include "utils/Utils.h"

#include "ExactAlgorithmManager.h"
#include "BenchmarkManager.h"
#include "HeuristicAlgorithmManager.h"

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
        print(e.what());
    }
    catch (...)
    {
        print("Nieznany błąd\n");
    }
    return -1;
}

auto App::mainMenu() -> void
{
    static constexpr const char* MAIN_MENU =
        "Wybierz tryb:\n"
        "1. Tryb algorytmów dokładnych\n"
        "2. Tryb algorytmów heurystycznych\n"
        "3. Tryb benchmarków\n"
        "4. Wyjście\n"
        "> ";

    while (true)
    {
        const auto choice = utils::getChoiceFromMenu(MAIN_MENU, 1, 4);
        switch (choice)
        {
        case 1:
            ExactAlgorithmManager{}.menu();
            break;
        case 2:
            HeuristicAlgorithmManager{}.menu();
            break;
        case 3:
            BenchmarkManager{}.menu();
            break;
        default:
            return;
        }
    }
}
}