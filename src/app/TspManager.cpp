#include "TspManager.h"
#include "utils/Utils.h"
#include "utils/Print.h"
#include "parsers/ProjectReader.h"

namespace qs
{

auto TspManager::printResult(
    const std::pair<algo::tsp::TspAlgorithm::Result, std::chrono::milliseconds>& result) -> void
{
    if (result.first.has_value())
    {
        print("Długość najkrótszej ścieżki wynosi: ", result.first->distance);
        print("Ścieżka: ", result.first->path);
        print("Czas: ", result.second.count(), "ms");
    }
    else
    {
        print("Nie udało się znaleźć najkrótszej ścieżki");
    }
}

auto TspManager::readGraphFromFileMenu() -> void
{
    const auto filename = utils::getInput<std::string>(std::cin, "Podaj nazwę pliku: ");

    if (!filename.has_value())
    {
        print("Błąd odczytu nazwy");
        return;
    }

    const auto fileContent = utils::getFileContent(filename.value());

    if (!fileContent.has_value())
    {
        print("Błąd odczytu pliku");
        return;
    }

    auto projectResult = getGraph(fileContent.value());
    if (projectResult.has_value())
    {
        graph = std::move(projectResult);
    }
    else
    {
        print("Nie udało się odczytać grafu");
        return;
    }
    print("Odczytano graf");
}

auto TspManager::printGraph() -> void
{
    if (graph.has_value())
    {
        print(graph->toString());
    }
    else
    {
        print("Nie załadowano grafu");
    }
}

}