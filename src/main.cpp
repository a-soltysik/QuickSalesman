#include "algorithms/tsp/BruteForce.h"
#include "algorithms/tsp/DynamicProgramming.h"

#include "utils/Utils.h"
//#include <windows.h>

int main()
{
    //SetConsoleOutputCP(CP_UTF8);
    //const auto a = tsplib::getTspContent("NAME:  br17\n"
    //                                     "TYPE: ATSP\n"
    //                                     "COMMENT: 17 city problem (Repetto)\n"
    //                                     "DIMENSION:  17\n"
    //                                     "EDGE_WEIGHT_TYPE: EXPLICIT\n"
    //                                     "EDGE_WEIGHT_FORMAT: FULL_MATRIX\n"
    //                                     "UNKNOWN_TAG1: aaa\n"
    //                                     "UNKNOWN_TAG2\n"
    //                                     "EDGE_WEIGHT_SECTION:\n"
    //                                     "0   1  2   3   4  5\n"
    //                                     "12  0 54   2   3  7\n"
    //                                     "3   4  0  45  23  9\n"
    //                                     "30 11  9   0  19 18\n"
    //                                     "1   9 30  24   0  9\n"
    //                                     "28  3 21  7  16   0\n"
    //                                     "EOF");

    auto graph = tsplib::Graph{6};
    graph.addEdge({{0, 1}, 1});
    graph.addEdge({{0, 2}, 2});
    graph.addEdge({{0, 3}, 3});
    graph.addEdge({{0, 4}, 4});
    graph.addEdge({{0, 5}, 5});

    graph.addEdge({{1, 0}, 12});
    graph.addEdge({{1, 2}, 54});
    graph.addEdge({{1, 3}, 2});
    graph.addEdge({{1, 4}, 3});
    graph.addEdge({{1, 5}, 7});

    graph.addEdge({{2, 0}, 3});
    graph.addEdge({{2, 1}, 4});
    graph.addEdge({{2, 3}, 45});
    graph.addEdge({{2, 4}, 23});
    graph.addEdge({{2, 5}, 9});

    graph.addEdge({{3, 0}, 30});
    graph.addEdge({{3, 1}, 11});
    graph.addEdge({{3, 2}, 9});
    graph.addEdge({{3, 4}, 19});
    graph.addEdge({{3, 5}, 18});

    graph.addEdge({{4, 0}, 1});
    graph.addEdge({{4, 1}, 9});
    graph.addEdge({{4, 2}, 30});
    graph.addEdge({{4, 3}, 24});
    graph.addEdge({{4, 5}, 9});

    graph.addEdge({{5, 0}, 28});
    graph.addEdge({{5, 1}, 3});
    graph.addEdge({{5, 2}, 21});
    graph.addEdge({{5, 3}, 7});
    graph.addEdge({{5, 4}, 16});

    //qs::utils::print(graph.toString());


        auto result1 = qs::algo::tsp::BruteForce{}.calculate(graph);
        qs::utils::print(result1.value().distance);
        qs::utils::print(result1.value().path);
        qs::utils::print(qs::algo::tsp::getPathLength(result1.value().path, graph));

        auto result2 = qs::algo::tsp::DynamicProgramming{}.calculate(graph);
        qs::utils::print(result2.value().distance);
        qs::utils::print(result2.value().path);
    qs::utils::print(qs::algo::tsp::getPathLength(result2.value().path, graph));
}