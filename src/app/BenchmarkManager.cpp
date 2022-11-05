#include "BenchmarkManager.h"

#include "algorithms/tsp/BruteForce.h"
#include "algorithms/tsp/DynamicProgramming.h"
#include "algorithms/tsp/BranchAndBound.h"

auto qs::BenchmarkManager::menu() -> void
{
    static constexpr const char* MENU =
                                   "Wybierz operację:\n"
                                   "1. Wybierz liczbę wierzchołków grafu\n"
                                   "2. Testuj algorytm typu Brute Force\n"
                                   "3. Testuj algorytm typu Dynamic Programming\n"
                                   "4. Testuj algorytm typu Branch and Bound (Low Cost)\n"
                                   "5. Testuj algorytm typu Branch and Bound (DFS)\n"
                                   "6. Wróć"
                                   "> ";

    while (true)
    {
        auto choice = utils::getChoiceFromMenu(MENU, 1, 5);
        switch (choice)
        {
        case 1:
            graphOrder = utils::getChoiceFromMenu("Wpisz liczbę wierzchołków <2, 50>: ", 2, 50);
            break;
        case 2:
            manageAlgorithm<algo::tsp::BruteForce>();
            break;
        case 3:
            manageAlgorithm<algo::tsp::DynamicProgramming>();
            break;
        case 4:
            manageAlgorithm(algo::tsp::bnb::lowCost);
            break;
        case 5:
            manageAlgorithm(algo::tsp::bnb::depthFirstSearch);
            break;
        default:
            return;
        }
    }
}
