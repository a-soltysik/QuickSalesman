#include "HeuristicAlgorithmManager.h"

#include "algorithms/tsp/TabuSearch.h"
#include "algorithms/tsp/SimulatedAnnealing.h"

namespace qs
{

auto HeuristicAlgorithmManager::menu() -> void
{
    static constexpr const char* MENU =
                                   "Wybierz operację:\n"
                                   "1. Odczytaj graf z pliku\n"
                                   "2. Wyświetl graf\n"
                                   "3. Ustaw warunek stopu\n"
                                   "4. Wybierz sąsiedztwo\n"
                                   "5. Znajdź najkrótszą ścieżkę algorytmem typu Tabu Search\n"
                                   "6. Znajdź najkrótszą ścieżkę algorytmem typu Simulated Annealing\n"
                                   "7. Wróć"
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
            stopMenu();
            break;
        case 4:
            neighbourhoodMenu();
            break;
        case 5:
            if (checkParameters())
            {
                manageAlgorithm(algo::tsp::TabuSearch{getNeighbourhood.value(),
                                                      algo::tsp::randomBasicSolution,
                                                      100,
                                                      time.value()});
            }
            break;
        case 6:
            if (checkParameters())
            {
                manageAlgorithm(algo::tsp::SimulatedAnnealing{getNeighbourhood.value(),
                                                              algo::tsp::randomBasicSolution,
                                                              time.value()});
            }
            break;
        default:
            return;
        }
    }
}

auto HeuristicAlgorithmManager::stopMenu() -> void
{
    const auto result = utils::getInput<uint32_t>(std::cin, "Podaj warunek stopu w sekundach: ");
    if (result.has_value())
    {
        time = std::chrono::seconds {result.value()};
    }
    else
    {
        print("Niepoprawna liczba");
    }
}

auto HeuristicAlgorithmManager::neighbourhoodMenu() -> void
{
    static constexpr const char* MENU =
                                   "Wybierz sąsiedztwo:\n"
                                   "1. Losowa zamiana wierzchołków\n"
                                   "2. Losowe odwrócenie części ścieżki\n"
                                   "3. Wróć"
                                   "> ";

    const auto choice = utils::getChoiceFromMenu(MENU, 1, 3);
    switch (choice)
    {
    case 1:
        getNeighbourhood = algo::tsp::randomSwap;
        break;
    case 2:
        getNeighbourhood = algo::tsp::randomRangeReverse;
        break;
    default:
        return;
    }
}

auto HeuristicAlgorithmManager::checkParameters() -> bool
{
    if (!time.has_value())
    {
        print("Warunek stopu nie został ustawiony");
        return false;
    }
    if (!getNeighbourhood.has_value())
    {
        print("Sąsiedztwo nie zostało wybrane");
        return false;
    }
    return true;
}

}