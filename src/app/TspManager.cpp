#include "TspManager.h"
#include "utils/Utils.h"
#include "utils/Print.h"
#include "parsers/ProjectReader.h"

#include "algorithms/tsp/BruteForce.h"
#include "algorithms/tsp/DynamicProgramming.h"
#include "algorithms/tsp/BranchAndBound.h"

namespace qs
{

auto TspManager::menu() -> void
{
    static constexpr const char* MENU =
                                   "Wybierz operację:\n"
                                   "1. Odczytaj graf z pliku\n"
                                   "2. Wyświetl graf\n"
                                   "3. Znajdź najkrótszą ścieżkę algorytmem typu Brute Force\n"
                                   "4. Znajdź najkrótszą ścieżkę algorytmem typu Dynamic Programming\n"
                                   "5. Znajdź najkrótszą ścieżkę algorytmem typu Branch and Bound\n"
                                   "6. Wróć"
                                   "> ";

    while (true)
    {
        auto choice = utils::getChoiceFromMenu(MENU, 1, 6);
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
            manageAlgorithm<algo::tsp::BranchAndBound>();
            break;
        default:
            return;
        }
    }
}

auto TspManager::printResult(const std::optional<algo::tsp::TspResult>& result) -> void
{
    if (result.has_value())
    {
        utils::print("Długość najkrótszej ścieżki wynosi: ", result->distance);
        utils::print("Ścieżka: ", result->path);
    }
    else
    {
        utils::print("Nie udało się znaleźć najkrótszej ścieżki");
    }
}

auto TspManager::readGraphFromFileMenu() -> void
{
#if defined(NDEBUG)
    std::cout << "Podaj nazwę pliku: ";
    const auto filename = utils::getInput<std::string>(std::cin);

    if (!filename.has_value())
    {
        utils::print("Błąd odczytu nazwy");
        return;
    }

    const auto fileContent = utils::getFileContent(filename.value());

    if (!fileContent.has_value())
    {
        utils::print("Błąd odczytu pliku");
        return;
    }

    auto projectResult = getGraph(fileContent.value());
    if (projectResult.has_value())
    {
        graph = std::move(projectResult);
    }
    else
    {
        auto tsplibResult = tsplib::getTspContent(fileContent.value());
        if (tsplibResult.has_value())
        {
            if (tsplibResult->metaData.name.has_value())
            {
                utils::print("Nazwa: ", tsplibResult->metaData.name.value());
            }
            if (tsplibResult->metaData.comment.has_value())
            {
                utils::print("Komentarz: ", tsplibResult->metaData.comment.value());
            }
            graph = std::move(tsplibResult->graph);
        }
        else
        {
            utils::print("Nie udało się odczytać grafu");
            return;
        }
    }
    utils::print("Odczytano graf");
#endif
}

auto TspManager::printGraph() -> void
{
    if (graph.has_value())
    {
        utils::print(graph->toString());
    }
    else
    {
        utils::print("Nie załadowano grafu");
    }
}

}