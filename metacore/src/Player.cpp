#include "Player.h"
#include "overloaded.h"

namespace metacore {

namespace {

constexpr auto player_move_increment = 50;

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
    return std::visit(
        overloaded{
            [](SlashAttackMechanic const& attack) -> bool {
                return attack.is_active();
            },
            [](auto const&) -> bool { return false; }},
        attack_);
}

namespace {

void tick(std::variant<std::monostate, SlashAttackMechanic>& attack)
{
    return std::visit(
        overloaded{
            [](SlashAttackMechanic& attack) { attack.tick(); },
            [](auto const&) {}},
        attack);
}

} // namespace

void Player::move_up()
{
    position_.y += player_move_increment;
    tick(attack_);
}

void Player::move_down()
{
    position_.y -= player_move_increment;
    tick(attack_);
}

void Player::move_right()
{
    position_.x += player_move_increment;
    tick(attack_);
}

void Player::move_left()
{
    position_.x -= player_move_increment;
    tick(attack_);
}

void Player::attack()
{
    std::visit(
        overloaded{
            [](SlashAttackMechanic& attack) { attack.start(); },
            [](auto const&) {}},
        attack_);
}

void Player::set_attack(AttackUpgrade const upgrade)
{
    switch (upgrade) {
        case AttackUpgrade::Slash:
            attack_ = SlashAttackMechanic{};
            break;
        case AttackUpgrade::Shoot:
            break;
    }
}

} // namespace metacore