#pragma once

#include "TspAlgorithm.h"
#include "algorithms/utils/PathMask.h"
#include "utils/time/BasicTimer.h"

#include <array>

namespace qs::algo::tsp
{

template<typename T>
concept NeighbourhoodGetter =
requires(T t, typename T::NeighbourDescription nd1, typename T::NeighbourDescription nd2)
{
    { nd1 == nd2 } -> std::convertible_to<bool>;
    { t.getNextNeighbour() } -> std::convertible_to<std::pair<typename T::NeighbourDescription, TspResult::Distance>>;
    { t.hasNextNeighbour() } -> std::convertible_to<bool>;
    { t.getNeighbourByDescription(nd1) } -> std::convertible_to<TspResult::Path>;
};

template<NeighbourhoodGetter GetNeighbourhood>
class TabuSearch : public TspAlgorithm
{
public:
    TabuSearch(BasicSolutionGetter basicSolutionGetter, std::chrono::seconds time);

    [[nodiscard]]
    auto solve(const tsplib::Graph& graph) -> Result override;

private:
    using NeighbourDescription = typename GetNeighbourhood::NeighbourDescription;
    using Neighbour = std::pair<NeighbourDescription, TspResult::Distance>;
    using Candidate = std::pair<NeighbourDescription, TspResult>;

    auto setBestSolution(TspResult& bestSolution, const TspResult& bestCandidate) -> void;
    auto updateTabuList(const NeighbourDescription& bestCandidate) -> void;

    auto findBestCandidate(const TspResult& state) -> Candidate;

    auto makeCandidate(const Neighbour& neighbour, const GetNeighbourhood& getNeighbourhood) -> Candidate;

    BasicSolutionGetter  getBasicSolution;
    std::chrono::seconds time;

    const tsplib::Graph* currentGraph;
    std::vector<NeighbourDescription> tabuList;
};

template<NeighbourhoodGetter GetNeighbourhood>
TabuSearch<GetNeighbourhood>::TabuSearch(BasicSolutionGetter basicSolutionGetter,
                                         std::chrono::seconds time)
    : getBasicSolution(std::move(basicSolutionGetter))
    , time {time}
    , currentGraph {}
{

}

template<NeighbourhoodGetter GetNeighbourhood>
auto TabuSearch<GetNeighbourhood>::solve(const tsplib::Graph& graph) -> Result
{
    currentGraph = &graph;
    tabuList     = std::vector(graph.getOrder() / 2, NeighbourDescription{});

    auto basicTimer = qs::utils::BasicTimer(time);
    basicTimer.start();

    auto bestSolution  = makeTspResult(getBasicSolution(graph), graph);
    auto bestCandidate = Candidate {{}, bestSolution};

    while (basicTimer.isRunning())
    {
        bestCandidate = findBestCandidate(bestCandidate.second);
        setBestSolution(bestSolution, bestCandidate.second);
        updateTabuList(bestCandidate.first);
    }

    return bestSolution;
}

template<NeighbourhoodGetter GetNeighbourhood>
auto TabuSearch<GetNeighbourhood>::findBestCandidate(const TspResult& state) -> Candidate
{
    auto getNeighbourhood = GetNeighbourhood {state, *currentGraph};
    if (!getNeighbourhood.hasNextNeighbour())
    {
        return {{}, makeTspResult(getBasicSolution(*currentGraph), *currentGraph)};
    }

    auto bestNeighbourSet = false;
    auto bestNeighbour    = Neighbour {};

    while (getNeighbourhood.hasNextNeighbour())
    {
        const auto neighbour = getNeighbourhood.getNextNeighbour();
        if (std::ranges::find(tabuList, neighbour.first) == tabuList.cend())
        {
            if (!bestNeighbourSet || (neighbour.second < bestNeighbour.second))
            {
                bestNeighbour    = neighbour;
                bestNeighbourSet = true;
            }
        }
    }

    if (!bestNeighbourSet) [[unlikely]]
    {
        return {{}, makeTspResult(getBasicSolution(*currentGraph), *currentGraph)};
    }

    return  makeCandidate(bestNeighbour, getNeighbourhood);;
}

template<NeighbourhoodGetter GetNeighbourhood>
auto TabuSearch<GetNeighbourhood>::updateTabuList(const NeighbourDescription& bestCandidate) -> void
{
    std::ranges::rotate(tabuList, tabuList.begin() + 1);
    tabuList.back() = bestCandidate;
}

template<NeighbourhoodGetter GetNeighbourhood>
auto TabuSearch<GetNeighbourhood>::setBestSolution(TspResult& bestSolution, const TspResult& bestCandidate) -> void
{
    if (bestCandidate.distance < bestSolution.distance)
    {
        bestSolution = bestCandidate;
    }
}

template<NeighbourhoodGetter GetNeighbourhood>
auto TabuSearch<GetNeighbourhood>::makeCandidate(const Neighbour& neighbour,
                                                 const GetNeighbourhood& getNeighbourhood) -> std::pair<NeighbourDescription, TspResult>
{
    return {neighbour.first, makeTspResult(getNeighbourhood.getNeighbourByDescription(neighbour.first), *currentGraph)};
}

}
