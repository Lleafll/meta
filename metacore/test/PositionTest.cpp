#include "Position.h"
#include <gtest/gtest.h>

using namespace metacore;

namespace {

TEST(PositionTest, IsWithinDistance)
{
    EXPECT_TRUE(is_within_distance<50>(Position{0, 0}, Position{0, 0}));
    EXPECT_FALSE(is_within_distance<10>(Position{0, 0}, Position{20, 20}));
    EXPECT_TRUE(is_within_distance<10>(Position{0, 0}, Position{10, 10}));
    EXPECT_TRUE(is_within_distance<10>(Position{10, 10}, Position{0, 0}));
}

} // namespace
