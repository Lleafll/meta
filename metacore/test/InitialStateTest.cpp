#include "InitialState.h"
#include "GameState.h"
#include <gtest/gtest.h>

using namespace metacore;

namespace {

TEST(InitialStateTest, ToGameState)
{
    auto const state = InitialState{
        Player{Position{12, 54}}, Pickup{Position{45, 76}, {{}, {}}}};
    auto const game_state = to_game_state(state);
    auto const expected = GameState{Position{12, 54}, Position{45, 76}};
    EXPECT_EQ(expected, game_state);
}

} // namespace