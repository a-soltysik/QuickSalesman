#include <gtest/gtest.h>
#include <cmath>

#include "algorithms/tsp/Utils.h"

TEST(UtilsTest, PathMaskValue)
{
    using qs::algo::tsp::utils::PathMask;
    EXPECT_EQ(PathMask{123}.value(), 123);
    EXPECT_EQ(PathMask{std::numeric_limits<uint64_t>::max()}.value(), std::numeric_limits<uint64_t>::max());
}

TEST(UtilsTest, PathMaskVisited)
{
    using namespace qs::algo::tsp::utils;

    auto pathMask = allVisited(64);
    EXPECT_EQ(pathMask.value(), std::numeric_limits<uint64_t>::max());

    pathMask = allVisited(16);
    EXPECT_EQ(pathMask.value(), std::pow(2, 16) - 1);

    pathMask = pathMask.unvisit(8);
    EXPECT_FALSE(pathMask.isVisited(8));
    EXPECT_TRUE(pathMask.isVisited(9));

    pathMask = pathMask.unvisit(10);
    EXPECT_FALSE(pathMask.isVisited(10));
    EXPECT_FALSE(pathMask.isVisited(8));
    EXPECT_TRUE(pathMask.isVisited(9));

    EXPECT_EQ(pathMask.value(), 0b1111101011111111);

}

