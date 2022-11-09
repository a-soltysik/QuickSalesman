#include <gtest/gtest.h>

#include "algorithms/tsp/BruteForce.h"
#include "algorithms/tsp/DynamicProgramming.h"
#include "algorithms/tsp/BranchAndBound.h"
#include "parsers/ProjectReader.h"

auto getGraph6() -> std::pair<tsplib::Graph, qs::algo::tsp::TspResult::Distance>
{
    using namespace std::string_view_literals;
    static constexpr std::string_view data = "6\n"
                                             " 0 20 30 31 28 40\n"
                                             "30  0 10 14 20 44\n"
                                             "40 20  0 10 22 50\n"
                                             "41 24 20  0 14 42\n"
                                             "38 30 32 24  0 28\n"
                                             "50 54 60 52 38  0"sv;

    auto graph = qs::getGraph(data);
    return {
        graph.value(),
        132
    };
}

auto getGraph10() -> std::pair<tsplib::Graph, qs::algo::tsp::TspResult::Distance>
{
    using namespace std::string_view_literals;
    static constexpr std::string_view data = "10\n"
                                             " -1  81  50  18  75  39 107  77  87  43  \n"
                                             " 81  -1  76  21  37  26  34  58  66  15  \n"
                                             " 50  76  -1  24  14  58 100  68  33  30  \n"
                                             " 18  21  24  -1  19  58  68  62  84  81  \n"
                                             " 75  37  14  19  -1  31  60  65  29  91  \n"
                                             " 39  26  58  58  31  -1  64  21  42  46  \n"
                                             "107  34 100  68  60  64  -1  15  55  16  \n"
                                             " 77  58  68  62  65  21  15  -1  17  34  \n"
                                             " 87  66  33  84  29  42  55  17  -1  68  \n"
                                             " 43  15  30  81  91  46  16  34  68  -1"sv;

    auto graph = qs::getGraph(data);
    return {
        graph.value(),
        212
    };
}

auto getGraph12() -> std::pair<tsplib::Graph, qs::algo::tsp::TspResult::Distance>
{
    using namespace std::string_view_literals;
    static constexpr std::string_view data = "12\n"
                                             "-1 29 82 46 68 52 72 42 51 55 29 74\n"
                                             "29 -1 55 46 42 43 43 23 23 31 41 51\n"
                                             "82 55 -1 68 46 55 23 43 41 29 79 21\n"
                                             "46 46 68 -1 82 15 72 31 62 42 21 51\n"
                                             "68 42 46 82 -1 74 23 52 21 46 82 58\n"
                                             "52 43 55 15 74 -1 61 23 55 31 33 37\n"
                                             "72 43 23 72 23 61 -1 42 23 31 77 37\n"
                                             "42 23 43 31 52 23 42 -1 33 15 37 33\n"
                                             "51 23 41 62 21 55 23 33 -1 29 62 46\n"
                                             "55 31 29 42 46 31 31 15 29 -1 51 21\n"
                                             "29 41 79 21 82 33 77 37 62 51 -1 65\n"
                                             "74 51 21 51 58 37 37 33 46 21 65 -1"sv;

    auto graph = qs::getGraph(data);
    return {
        graph.value(),
        264
    };
}

auto getGraph13() -> std::pair<tsplib::Graph, qs::algo::tsp::TspResult::Distance>
{
    using namespace std::string_view_literals;
    static constexpr std::string_view data = "13\n"
                                             "-1 29 82 46 68 52 72 42 51 55 29 74 23\n"
                                             "29 -1 55 46 42 43 43 23 23 31 41 51 11\n"
                                             "82 55 -1 68 46 55 23 43 41 29 79 21 64\n"
                                             "46 46 68 -1 82 15 72 31 62 42 21 51 51\n"
                                             "68 42 46 82 -1 74 23 52 21 46 82 58 46\n"
                                             "52 43 55 15 74 -1 61 23 55 31 33 37 51\n"
                                             "72 43 23 72 23 61 -1 42 23 31 77 37 51\n"
                                             "42 23 43 31 52 23 42 -1 33 15 37 33 33\n"
                                             "51 23 41 62 21 55 23 33 -1 29 62 46 29\n"
                                             "55 31 29 42 46 31 31 15 29 -1 51 21 41\n"
                                             "29 41 79 21 82 33 77 37 62 51 -1 65 42\n"
                                             "74 51 21 51 58 37 37 33 46 21 65 -1 61\n"
                                             "23 11 64 51 46 51 51 33 29 41 42 61 -1"sv;

    auto graph = qs::getGraph(data);
    return {
        graph.value(),
        269
    };
}

auto getGraph14() -> std::pair<tsplib::Graph, qs::algo::tsp::TspResult::Distance>
{
    using namespace std::string_view_literals;
    static constexpr std::string_view data = "14\n"
                                             "-1 29 82 46 68 52 72 42 51 55 29 74 23 72\n"
                                             "29 -1 55 46 42 43 43 23 23 31 41 51 11 52\n"
                                             "82 55 -1 68 46 55 23 43 41 29 79 21 64 31\n"
                                             "46 46 68 -1 82 15 72 31 62 42 21 51 51 43\n"
                                             "68 42 46 82 -1 74 23 52 21 46 82 58 46 65\n"
                                             "52 43 55 15 74 -1 61 23 55 31 33 37 51 29\n"
                                             "72 43 23 72 23 61 -1 42 23 31 77 37 51 46\n"
                                             "42 23 43 31 52 23 42 -1 33 15 37 33 33 31\n"
                                             "51 23 41 62 21 55 23 33 -1 29 62 46 29 51\n"
                                             "55 31 29 42 46 31 31 15 29 -1 51 21 41 23\n"
                                             "29 41 79 21 82 33 77 37 62 51 -1 65 42 59\n"
                                             "74 51 21 51 58 37 37 33 46 21 65 -1 61 11\n"
                                             "23 11 64 51 46 51 51 33 29 41 42 61 -1 62\n"
                                             "72 52 31 43 65 29 46 31 51 23 59 11 62 -1"sv;

    auto graph = qs::getGraph(data);
    return {
        graph.value(),
        282
    };
}

auto getGraph15() -> std::pair<tsplib::Graph, qs::algo::tsp::TspResult::Distance>
{
    using namespace std::string_view_literals;
    static constexpr std::string_view data = "15\n"
                                             "-1 29 82 46 68 52 72 42 51 55 29 74 23 72 46\n"
                                             "29 -1 55 46 42 43 43 23 23 31 41 51 11 52 21\n"
                                             "82 55 -1 68 46 55 23 43 41 29 79 21 64 31 51\n"
                                             "46 46 68 -1 82 15 72 31 62 42 21 51 51 43 64\n"
                                             "68 42 46 82 -1 74 23 52 21 46 82 58 46 65 23\n"
                                             "52 43 55 15 74 -1 61 23 55 31 33 37 51 29 59\n"
                                             "72 43 23 72 23 61 -1 42 23 31 77 37 51 46 33\n"
                                             "42 23 43 31 52 23 42 -1 33 15 37 33 33 31 37\n"
                                             "51 23 41 62 21 55 23 33 -1 29 62 46 29 51 11\n"
                                             "55 31 29 42 46 31 31 15 29 -1 51 21 41 23 37\n"
                                             "29 41 79 21 82 33 77 37 62 51 -1 65 42 59 61\n"
                                             "74 51 21 51 58 37 37 33 46 21 65 -1 61 11 55\n"
                                             "23 11 64 51 46 51 51 33 29 41 42 61 -1 62 23\n"
                                             "72 52 31 43 65 29 46 31 51 23 59 11 62 -1 59\n"
                                             "46 21 51 64 23 59 33 37 11 37 61 55 23 59 -1"sv;

    auto graph = qs::getGraph(data);
    return {
        graph.value(),
        291
    };
}

TEST(AlgorithmsTest, BruteForce)
{
    auto data   = getGraph6();
    auto result = qs::algo::tsp::BruteForce {}.solve(data.first);
    EXPECT_EQ(data.second, result->distance);
    EXPECT_EQ(data.second, qs::algo::tsp::getPathLength(result->path, data.first));

    data   = getGraph10();
    result = qs::algo::tsp::BruteForce {}.solve(data.first);
    EXPECT_EQ(data.second, result->distance);
    EXPECT_EQ(data.second, qs::algo::tsp::getPathLength(result->path, data.first));
}

TEST(AlgorithmsTest, DynamicProgramming)
{
    auto data   = getGraph6();
    auto result = qs::algo::tsp::DynamicProgramming {}.solve(data.first);
    EXPECT_EQ(data.second, result->distance);
    EXPECT_EQ(data.second, qs::algo::tsp::getPathLength(result->path, data.first));

    data   = getGraph10();
    result = qs::algo::tsp::DynamicProgramming {}.solve(data.first);
    EXPECT_EQ(data.second, result->distance);
    EXPECT_EQ(data.second, qs::algo::tsp::getPathLength(result->path, data.first));

    data   = getGraph12();
    result = qs::algo::tsp::DynamicProgramming {}.solve(data.first);
    EXPECT_EQ(data.second, result->distance);
    EXPECT_EQ(data.second, qs::algo::tsp::getPathLength(result->path, data.first));

    data   = getGraph13();
    result = qs::algo::tsp::DynamicProgramming {}.solve(data.first);
    EXPECT_EQ(data.second, result->distance);
    EXPECT_EQ(data.second, qs::algo::tsp::getPathLength(result->path, data.first));

    data   = getGraph14();
    result = qs::algo::tsp::DynamicProgramming {}.solve(data.first);
    EXPECT_EQ(data.second, result->distance);
    EXPECT_EQ(data.second, qs::algo::tsp::getPathLength(result->path, data.first));

    data   = getGraph15();
    result = qs::algo::tsp::DynamicProgramming {}.solve(data.first);
    EXPECT_EQ(data.second, result->distance);
    EXPECT_EQ(data.second, qs::algo::tsp::getPathLength(result->path, data.first));
}

TEST(AlgorithmsTest, BnB_LowCost)
{
    auto data   = getGraph6();
    auto result = qs::algo::tsp::BranchAndBound {qs::algo::tsp::bnb::lowCost}.solve(data.first);
    EXPECT_EQ(data.second, result->distance);
    EXPECT_EQ(data.second, qs::algo::tsp::getPathLength(result->path, data.first));

    data   = getGraph10();
    result = qs::algo::tsp::BranchAndBound {qs::algo::tsp::bnb::lowCost}.solve(data.first);
    EXPECT_EQ(data.second, result->distance);
    EXPECT_EQ(data.second, qs::algo::tsp::getPathLength(result->path, data.first));

    data   = getGraph12();
    result = qs::algo::tsp::BranchAndBound {qs::algo::tsp::bnb::lowCost}.solve(data.first);
    EXPECT_EQ(data.second, result->distance);
    EXPECT_EQ(data.second, qs::algo::tsp::getPathLength(result->path, data.first));

    data   = getGraph13();
    result = qs::algo::tsp::BranchAndBound {qs::algo::tsp::bnb::lowCost}.solve(data.first);
    EXPECT_EQ(data.second, result->distance);
    EXPECT_EQ(data.second, qs::algo::tsp::getPathLength(result->path, data.first));

    data   = getGraph14();
    result = qs::algo::tsp::BranchAndBound {qs::algo::tsp::bnb::lowCost}.solve(data.first);
    EXPECT_EQ(data.second, result->distance);
    EXPECT_EQ(data.second, qs::algo::tsp::getPathLength(result->path, data.first));

    data   = getGraph15();
    result = qs::algo::tsp::BranchAndBound {qs::algo::tsp::bnb::lowCost}.solve(data.first);
    EXPECT_EQ(data.second, result->distance);
    EXPECT_EQ(data.second, qs::algo::tsp::getPathLength(result->path, data.first));
}

TEST(AlgorithmsTest, BnB_DFS)
{
    auto data   = getGraph6();
    auto result = qs::algo::tsp::BranchAndBound {qs::algo::tsp::bnb::depthFirstSearch}.solve(data.first);
    EXPECT_EQ(data.second, result->distance);
    EXPECT_EQ(data.second, qs::algo::tsp::getPathLength(result->path, data.first));

    data   = getGraph10();
    result = qs::algo::tsp::BranchAndBound {qs::algo::tsp::bnb::depthFirstSearch}.solve(data.first);
    EXPECT_EQ(data.second, result->distance);
    EXPECT_EQ(data.second, qs::algo::tsp::getPathLength(result->path, data.first));

    data   = getGraph12();
    result = qs::algo::tsp::BranchAndBound {qs::algo::tsp::bnb::depthFirstSearch}.solve(data.first);
    EXPECT_EQ(data.second, result->distance);
    EXPECT_EQ(data.second, qs::algo::tsp::getPathLength(result->path, data.first));

    data   = getGraph13();
    result = qs::algo::tsp::BranchAndBound {qs::algo::tsp::bnb::depthFirstSearch}.solve(data.first);
    EXPECT_EQ(data.second, result->distance);
    EXPECT_EQ(data.second, qs::algo::tsp::getPathLength(result->path, data.first));

    data   = getGraph14();
    result = qs::algo::tsp::BranchAndBound {qs::algo::tsp::bnb::depthFirstSearch}.solve(data.first);
    EXPECT_EQ(data.second, result->distance);
    EXPECT_EQ(data.second, qs::algo::tsp::getPathLength(result->path, data.first));

    data   = getGraph15();
    result = qs::algo::tsp::BranchAndBound {qs::algo::tsp::bnb::depthFirstSearch}.solve(data.first);
    EXPECT_EQ(data.second, result->distance);
    EXPECT_EQ(data.second, qs::algo::tsp::getPathLength(result->path, data.first));
}