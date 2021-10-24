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

bool Player::is_slashing() const
{
    return false;
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

void Player::attack()
{
}

void Player::set_attack(AttackUpgrade const upgrade)
{
}

} // namespace metacore