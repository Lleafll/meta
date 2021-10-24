#include "Player.h"

namespace metacore {

namespace {

constexpr auto player_move_increment = 5;

} // namespace

Player::Player(Position const& position) : position_{position}
{
}

Position const& Player::position() const
{
    return position_;
}

void Player::move_up()
{
    position_.y += player_move_increment;
}

void Player::move_down()
{
    position_.y -= player_move_increment;
}

void Player::move_right()
{
    position_.x += player_move_increment;
}

void Player::move_left()
{
    position_.x -= player_move_increment;
}

} // namespace metacore