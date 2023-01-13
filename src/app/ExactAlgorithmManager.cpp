#include "ExactAlgorithmManager.h"

#include "algorithms/tsp/BruteForce.h"
#include "algorithms/tsp/DynamicProgramming.h"
#include "algorithms/tsp/BranchAndBound.h"

namespace qs
{

auto ExactAlgorithmManager::menu() -> void
{
    using namespace std::chrono_literals;
    static constexpr const char* MENU =
                                   "Wybierz operację:\n"
                                   "1. Odczytaj graf z pliku\n"
                                   "2. Wyświetl graf\n"
                                   "3. Znajdź najkrótszą ścieżkę algorytmem typu Brute Force\n"
                                   "4. Znajdź najkrótszą ścieżkę algorytmem typu Dynamic Programming\n"
                                   "5. Znajdź najkrótszą ścieżkę algorytmem typu Branch and Bound (Low Cost)\n"
                                   "6. Znajdź najkrótszą ścieżkę algorytmem typu Branch and Bound (DFS)\n"
                                   "7. Wróć\n"
                                   "> ";

    while (true)
    {
        const auto choice = utils::getChoiceFromMenu(MENU, 1, 7);
        switch (choice)
        {
        case 1:
            readGraphFromFileMenu();
            break;
        case 2:
            printGraph();
            break;
        case 3:
            manageAlgorithm<algo::tsp::BruteForce>();
            break;
        case 4:
            manageAlgorithm<algo::tsp::DynamicProgramming>();
            break;
        case 5:
            manageAlgorithm(algo::tsp::bnb::lowCost);
            break;
        case 6:
            manageAlgorithm(algo::tsp::bnb::depthFirstSearch);
            break;
        default:
            return;
        }
    }
}

}