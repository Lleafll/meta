#include "MetaEngine.h"
#include <gtest/gtest.h>

using namespace metacore;

namespace {

TEST(MetaEngineTest, InputRightOnFreshEngine)
{
    auto engine = MetaEngine{};
    ASSERT_EQ((Position{0.0, 0.0}), (engine.calculate_state().player_position));
    engine.input_right();
    auto const position = engine.calculate_state().player_position;
    EXPECT_LT(0.0, position.x);
    EXPECT_EQ(0.0, position.y);
}

} // namespace