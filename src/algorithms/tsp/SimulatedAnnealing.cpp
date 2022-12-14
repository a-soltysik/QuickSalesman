#include "SimulatedAnnealing.h"
#include "utils/time/BasicTimer.h"
#include "utils/Utils.h"
#include "utils/Print.h"

#include <cmath>
#include <utility>

namespace qs::algo::tsp
{

SimulatedAnnealing::SimulatedAnnealing(TspAlgorithm::NeighbourhoodGetter neighbourhoodGetter,
                                       TspAlgorithm::BasicSolutionGetter basicSolutionGetter, std::chrono::seconds time)
    : getNeighbours{std::move(neighbourhoodGetter)}
    , getBasicSolution{std::move(basicSolutionGetter)}
    , time {time}
{

}

auto SimulatedAnnealing::solve(const tsplib::Graph& graph) -> Algorithm::Result
{
    auto basicTimer = qs::utils::BasicTimer(time);
    basicTimer.start();

    auto bestSolution = getBasicSolution(graph);
    auto currentTemperature = static_cast<double>(graph.getOrder() * 100);

    static constexpr auto alpha = 0.999997;

    while (basicTimer.isRunning())
    {
        const auto neighbour = getNeighbours(bestSolution, 1).front();
        const auto costDifference = getCostDifference(neighbour, bestSolution, graph);
        if (costDifference < 0)
        {
            bestSolution = neighbour;
        }
        else
        {
            if (qs::utils::choose(getProbability(costDifference, currentTemperature)))
            {
                bestSolution = neighbour;
            }
        }
        currentTemperature *= alpha;
    }
    return {{
                bestSolution,
                getPathLength(bestSolution, graph)
            }};
}

auto SimulatedAnnealing::getCostDifference(const TspResult::Path& state,
                                           const TspResult::Path& bestSolution,
                                           const tsplib::Graph& graph) -> int64_t
{
    return static_cast<int64_t>(getPathLength(state, graph)) - static_cast<int64_t>(getPathLength(bestSolution, graph));
}

auto SimulatedAnnealing::getProbability(int64_t costDifference, double temperature) -> double
{
    return std::exp(-static_cast<double>(costDifference) / temperature);
}

}