#include "Player.h"
#include "overloaded.h"
#include <algorithm>

namespace metacore {

namespace {

constexpr auto player_move_increment = 50;
constexpr auto slash_radius = 100;
constexpr auto shoot_kill_radius = 50;
constexpr auto player_size = 49;

} // namespace

Player::Player(Position const& position)
    : position_and_orientation_{position, Orientation::Up}
{
}

Player::Player(PositionAndOrientation const& position_and_orientation)
    : position_and_orientation_{position_and_orientation}
{
}

Position const& Player::position() const
{
    return position_and_orientation_.position;
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

std::vector<Position> const* Player::projectiles() const
{
    return std::visit(
        overloaded{
            [](ShootAttackMechanic const& attack)
                -> std::vector<Position> const* {
                return &attack.projectiles();
            },
            [](auto const&) -> std::vector<Position> const* {
                return nullptr;
            }},
        attack_);
} //

namespace {

void tick(
    std::variant<std::monostate, SlashAttackMechanic, ShootAttackMechanic>&
        attack)
{
    return std::visit(
        overloaded{
            [](SlashAttackMechanic& attack) { attack.tick(); },
            [](ShootAttackMechanic& attack) { attack.tick(); },
            [](auto const&) {}},
        attack);
}

void move_to_position_if_not_obstructed(
    PositionAndOrientation& old_value,
    PositionAndOrientation const& new_value,
    std::span<Tile const> const environment)
{
    if (std::ranges::none_of(
            environment, [&new_value](Tile const& tile) -> bool {
                return is_within_distance<player_size>(
                    new_value.position, tile.position);
            })) {
        old_value = new_value;
    }
}

} // namespace

void Player::move_up(std::span<Tile const> const environment)
{
    auto const& position = position_and_orientation_.position;
    move_to_position_if_not_obstructed(
        position_and_orientation_,
        {{position.x, position.y + player_move_increment}, Orientation::Up},
        environment);
    tick(attack_);
}

void Player::move_down(std::span<Tile const> const environment)
{
    position_and_orientation_.position.y -= player_move_increment;
    position_and_orientation_.orientation = Orientation::Down;
    tick(attack_);
}

void Player::move_right(std::span<Tile const> const environment)
{
    position_and_orientation_.position.x += player_move_increment;
    position_and_orientation_.orientation = Orientation::Right;
    tick(attack_);
}

void Player::move_left(std::span<Tile const> const environment)
{
    position_and_orientation_.position.x -= player_move_increment;
    position_and_orientation_.orientation = Orientation::Left;
    tick(attack_);
}

void Player::attack()
{
    tick(attack_);
    std::visit(
        overloaded{
            [](SlashAttackMechanic& attack) { attack.start(); },
            [this](ShootAttackMechanic& attack) {
                attack.start(position_and_orientation_);
            },
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
            attack_ = ShootAttackMechanic{};
            break;
    }
}

bool Player::target_is_hit(Position const& target) const
{
    return std::visit(
        overloaded{
            [this, &target](SlashAttackMechanic const& attack) -> bool {
                return attack.is_active() &&
                    is_within_distance<slash_radius>(
                           position_and_orientation_.position, target);
            },
            [&target](ShootAttackMechanic const& attack) -> bool {
                return std::ranges::any_of(
                    attack.projectiles(),
                    [&target](Position const& projectile) -> bool {
                        return is_within_distance<shoot_kill_radius>(
                            projectile, target);
                    });
            },
            [](auto const&) -> bool { return false; }},
        attack_);
}

} // namespace metacore