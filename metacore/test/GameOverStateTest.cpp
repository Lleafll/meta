#include "GameOverState.h"
#include <GameState.h>
#include <gtest/gtest.h>

using namespace metacore;

namespace {

TEST(GameOverStateTest, ToGameState)
{
    auto const state =
        GameOverState{Position{45, 78}, Position{65, 876}, {Position{7, 9}}};
    auto const game_state = to_game_state(state);
    auto const expected = GameState{
        Position{45, 78},
        Position{65, 876},
        std::nullopt,
        false,
        {Position{7, 9}},
        GameProgress::Lost};
    EXPECT_EQ(expected, game_state);
}

} // namespace