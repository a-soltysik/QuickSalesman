#include <gtest/gtest.h>

#include "algorithms/tsp/tabuSearch/RangeReverser.h"
#include "parsers/ProjectReader.h"

static auto getGraph6() -> std::pair<tsplib::Graph, qs::algo::tsp::TspResult::Distance>
{
    using namespace std::string_view_literals;
    static constexpr std::string_view data = "6\n"
                                             " 0 20 30 31 28 40\n"
                                             "30  0 10 14 20 44\n"
                                             "40 20  0 10 22 50\n"
                                             "41 24 20  0 14 42\n"
                                             "38 30 32 24  0 28\n"
                                             "50 54 60 52 38  0\n"sv;

    auto graph = qs::getGraph(data);
    return {
        graph.value(),
        132
    };
}

TEST(RangeReverserTest, GetNextNeighbour)
{
    const auto graph = getGraph6();
    const auto path  = qs::algo::tsp::makeTspResult({0, 1, 2, 3, 4, 5, 0}, graph.first);

    auto RangeReverser = qs::algo::tsp::ts::RangeReverser {path, graph.first};

    auto nextNeighbour = RangeReverser.getNextNeighbour();
    auto neighbourPath = RangeReverser.getNeighbourByDescription(nextNeighbour.first);

    EXPECT_EQ(qs::algo::tsp::getPathLength(neighbourPath, graph.first), nextNeighbour.second);

    nextNeighbour = RangeReverser.getNextNeighbour();
    neighbourPath = RangeReverser.getNeighbourByDescription(nextNeighbour.first);

    EXPECT_EQ(qs::algo::tsp::getPathLength(neighbourPath, graph.first), nextNeighbour.second);
}

