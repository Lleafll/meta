#include "Player.h"
#include "overloaded.h"
#include <algorithm>

namespace metacore {

namespace {

constexpr auto player_move_increment = 50;
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

Position& Player::position()
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

bool is_obstructed(
    PositionAndOrientation const& new_value,
    std::span<Tile const> const environment)
{
    return std::ranges::any_of(
        environment, [&new_value](Tile const& tile) -> bool {
            if (tile.type == TileType::Obstacle) {
                return is_within_distance<player_size>(
                    new_value.position, tile.position);
            }
            return false;
        });
}

} // namespace

void Player::move_up(std::span<Tile const> const environment)
{
    auto const& position = position_and_orientation_.position;
    if (!is_obstructed(
            {{position.x, position.y + player_move_increment}, Orientation::Up},
            environment)) {
        next_move_ = NextMove::Up;
    }
}

void Player::move_down(std::span<Tile const> const environment)
{
    auto const& position = position_and_orientation_.position;
    if (!is_obstructed(
            {{position.x, position.y - player_move_increment},
             Orientation::Down},
            environment)) {
        next_move_ = NextMove::Down;
    }
}

void Player::move_right(std::span<Tile const> const environment)
{
    auto const& position = position_and_orientation_.position;
    if (!is_obstructed(
            {{position.x + player_move_increment, position.y},
             Orientation::Right},
            environment)) {
        next_move_ = NextMove::Right;
    }
}

void Player::move_left(std::span<Tile const> const environment)
{
    auto const& position = position_and_orientation_.position;
    if (!is_obstructed(
            {{position.x - player_move_increment, position.y},
             Orientation::Left},
            environment)) {
        next_move_ = NextMove::Left;
    }
}

void Player::attack()
{
    next_move_ = NextMove::Attack;
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
            [&target](SlashAttackMechanic const& attack) -> bool {
                return attack.target_is_hit(target);
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

Orientation Player::orientation() const
{
    return position_and_orientation_.orientation;
}

void Player::advance()
{
    auto const& position = position_and_orientation_.position;
    switch (next_move_) {
        case NextMove::Up:
            position_and_orientation_ = {
                {position.x, position.y + player_move_increment},
                Orientation::Up};
            break;
        case NextMove::Down:
            position_and_orientation_ = {
                {position.x, position.y - player_move_increment},
                Orientation::Down};
            break;
        case NextMove::Right:
            position_and_orientation_ = {
                {position.x + player_move_increment, position.y},
                Orientation::Right};
            break;
        case NextMove::Left:
            position_and_orientation_ = {
                {position.x - player_move_increment, position.y},
                Orientation::Left};
            break;
        default:
            break;
    }
    std::visit(
        overloaded{
            [](SlashAttackMechanic& attack) { attack.tick(); },
            [](ShootAttackMechanic& attack) { attack.tick(); },
            [](auto const&) {}},
        attack_);
    if (next_move_ == NextMove::Attack) {
        std::visit(
            overloaded{
                [this](SlashAttackMechanic& attack) {
                    attack.start(position_and_orientation_);
                },
                [this](ShootAttackMechanic& attack) {
                    attack.start(position_and_orientation_);
                },
                [](std::monostate const&) {}},
            attack_);
    }
    next_move_ = NextMove::None;
}

} // namespace metacore