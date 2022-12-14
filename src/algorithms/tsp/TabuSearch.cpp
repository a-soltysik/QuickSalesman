#include "TabuSearch.h"
#include "utils/time/BasicTimer.h"

#include <algorithm>

namespace qs::algo::tsp
{

TabuSearch::TabuSearch(TabuSearch::NeighbourhoodGetter neighbourhoodGetter,
                       TabuSearch::BasicSolutionGetter basicSolutionGetter, size_t neighbourhoodSize,
                       std::chrono::seconds time)
    : getNeighbours(std::move(neighbourhoodGetter))
    , getBasicSolution(std::move(basicSolutionGetter))
    , neighbourhoodSize{neighbourhoodSize}
    , time{time}
{

}

auto TabuSearch::solve(const tsplib::Graph& graph) -> Result
{
    auto basicTimer = qs::utils::BasicTimer(time);
    basicTimer.start();

    auto bestCandidate = getBasicSolution(graph);
    auto bestSolution = bestCandidate;
    auto tabuList = std::deque{utils::toMask(bestCandidate)};

    while (basicTimer.isRunning())
    {
        bestCandidate = findBestCandidate(bestCandidate, tabuList, graph);
        setBestSolution(bestSolution, bestCandidate, graph);
        updateTabuList(tabuList, bestCandidate, graph);
    }
    return {{
        bestSolution,
        getPathLength(bestSolution, graph)
    }};
}

auto TabuSearch::fitness(const TspResult::Path& state, const tsplib::Graph& graph) -> TspResult::Distance
{
    return getPathLength(state, graph);
}

auto TabuSearch::findBestCandidate(const TspResult::Path& state, const std::deque<utils::PathMask>& tabuList, const tsplib::Graph& graph) -> TspResult::Path
{
    const auto neighbourhood = getNeighbours(state, neighbourhoodSize);
    auto bestCandidate = neighbourhood.front();
    for (const auto& candidate : neighbourhood)
    {
        if (std::ranges::find(tabuList, utils::toMask(candidate)) != tabuList.cend() &&
            fitness(candidate, graph) < fitness(bestCandidate, graph))
        {
            bestCandidate = candidate;
        }
    }
    return bestCandidate;
}

auto TabuSearch::updateTabuList(std::deque<utils::PathMask>& tabuList, const TspResult::Path& bestCandidate,
                                const tsplib::Graph& graph) -> void
{
    tabuList.push_back(utils::toMask(bestCandidate));
    if (tabuList.size() > graph.getOrder() * 10)
    {
        tabuList.pop_front();
    }
}

auto TabuSearch::setBestSolution(TspResult::Path& bestSolution, const TspResult::Path& bestCandidate,
                                 const tsplib::Graph& graph) -> void
{
    if (fitness(bestCandidate, graph) < fitness(bestSolution, graph))
    {
        bestSolution = bestCandidate;
    }
}

}