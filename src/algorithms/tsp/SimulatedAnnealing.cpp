#include "SimulatedAnnealing.h"
#include "utils/time/BasicTimer.h"
#include "utils/Utils.h"

namespace qs::algo::tsp
{

SimulatedAnnealing::SimulatedAnnealing(NeighbourGetter getNeighbour,
                                       BasicSolutionGetter getBasicSolution,
                                       std::chrono::seconds time)
    : getNeighbour {getNeighbour}
    , getBasicSolution {getBasicSolution}
    , time {time}
{

}

auto SimulatedAnnealing::solve(const tsplib::Graph& graph) -> Result
{
    auto basicTimer = qs::utils::BasicTimer(time);
    basicTimer.start();

    auto currentTemperature = getInitialTemperature(graph);
    auto bestSolution       = makeTspResult(getBasicSolution(graph), graph);
    auto currentSolution    = bestSolution;

    static constexpr auto alpha = 0.999997;

    while (basicTimer.isRunning())
    {
        const auto neighbour = makeTspResult(getNeighbour(currentSolution.path), graph);

        const auto costDifference = static_cast<int64_t>(neighbour.distance) -
                                    static_cast<int64_t>(currentSolution.distance);
        if (costDifference < 0)
        {
            currentSolution = neighbour;
            bestSolution    = neighbour;
        }
        else
        {
            if (qs::utils::choose(getProbability(costDifference, currentTemperature)))
            {
                currentSolution = neighbour;
            }
        }
        currentTemperature *= alpha;
    }
    return bestSolution;
}

auto SimulatedAnnealing::getProbability(int64_t costDifference, double temperature) -> double
{
    return std::exp(-static_cast<double>(costDifference) / temperature);
}

auto SimulatedAnnealing::getInitialTemperature(const tsplib::Graph& graph) -> double
{
    static constexpr auto numberOfIterations = 5;

    auto initialTemperature = 0.;

    for (auto i = uint32_t {}; i < numberOfIterations; i++)
    {
        initialTemperature += static_cast<double>(getPathLength(getBasicSolution(graph), graph));
    }
    return initialTemperature / numberOfIterations;
}

namespace sa
{

auto randomRangeReverse(const TspResult::Path& state) -> TspResult::Path
{
    auto index1 = size_t {};
    auto index2 = size_t {};

    while (index1 == index2)
    {
        index1 = qs::utils::getRandom<size_t>(1, state.size() - 2);
        index2 = qs::utils::getRandom<size_t>(1, state.size() - 2);
    }

    if (index1 > index2)
    {
        std::swap(index1, index2);
    }

    auto stateCopy = state;
    std::ranges::reverse(stateCopy.begin() + index1, stateCopy.begin() + index2);

    return stateCopy;
}

auto randomSwap(const TspResult::Path& state) -> TspResult::Path
{
    auto index1 = size_t {};
    auto index2 = size_t {};

    while (index1 == index2)
    {
        index1 = qs::utils::getRandom<size_t>(1, state.size() - 2);
        index2 = qs::utils::getRandom<size_t>(1, state.size() - 2);
    }

    auto stateCopy = state;
    std::swap(stateCopy[index1], stateCopy[index2]);

    return stateCopy;
}

}}