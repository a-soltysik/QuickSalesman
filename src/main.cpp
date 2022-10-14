#include "parsers/TspReader.h"
#include "algorithms/BruteForce.h"

#include <iostream>

int main()
{
    const auto a = tsplib::getTspContent("NAME:  br17\n"
                                         "TYPE: ATSP\n"
                                         "COMMENT: 17 city problem (Repetto)\n"
                                         "DIMENSION:  17\n"
                                         "EDGE_WEIGHT_TYPE: EXPLICIT\n"
                                         "EDGE_WEIGHT_FORMAT: FULL_MATRIX\n"
                                         "UNKNOWN_TAG1: aaa\n"
                                         "UNKNOWN_TAG2\n"
                                         "EDGE_WEIGHT_SECTION:\n"
                                         "0   1  2   3   4  5\n"
                                         "12  0 54   2   3  7\n"
                                         "3   4  0  45  23  9\n"
                                         "30 11  9   0  19 18\n"
                                         "1   9 30  24   0  9\n"
                                         "28  3 21  7  16   0\n"
                                         "EOF");

    if (a)
    {
        auto result = tsp::BruteForce().calculate(a->graph).value();
        std::cout << result.length << "\n";
        for (const auto v : result.path)
        {
            std::cout << v << " ";
        }
    }
}