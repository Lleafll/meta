#include "PositionD.h"
#include <gtest/gtest.h>

using namespace metacore;

namespace {

TEST(PositionDTest, IsWithinDistance)
{
    EXPECT_TRUE(is_within_distance(50.0, PositionD{0, 0}, PositionD{0, 0}));
    EXPECT_FALSE(is_within_distance(10.0, PositionD{0, 0}, PositionD{20, 20}));
    EXPECT_TRUE(is_within_distance(10.0, PositionD{0, 0}, PositionD{10, 10}));
    EXPECT_TRUE(is_within_distance(10.0, PositionD{10, 10}, PositionD{0, 0}));
}

TEST(PositionDTest, IsWithinDistanceOnlyWithX)
{
    EXPECT_FALSE(is_within_distance(50.0, PositionD{0, 0}, PositionD{10, 200}));
}

} // namespace
