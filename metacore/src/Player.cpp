#include "Player.h"
#include "overloaded.h"
#include <algorithm>
#include <gsl/gsl_util>

namespace metacore {

namespace {

constexpr auto player_move_speed = 2000.0 / 1000.0; // per millisecond
constexpr auto shoot_kill_radius = 50.0;
constexpr auto player_size = 49.0;

} // namespace

Player::Player(PositionD const& position)
    : position_and_orientation_{position, Orientation::Up}
{
}

Player::Player(PositionDAndOrientation const& position_and_orientation)
    : position_and_orientation_{position_and_orientation}
{
}

PositionD const& Player::position() const
{
    return position_and_orientation_.position;
}

PositionD& Player::position()
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

std::vector<PositionD> const* Player::projectiles() const
{
    return std::visit(
        overloaded{
            [](ShootAttackMechanic const& attack)
                -> std::vector<PositionD> const* {
                return &attack.projectiles();
            },
            [](auto const&) -> std::vector<PositionD> const* {
                return nullptr;
            }},
        attack_);
} //

void Player::move_up()
{
    next_move_ = NextMove::Up;
}

void Player::move_down()
{
    next_move_ = NextMove::Down;
}

void Player::move_right()
{
    next_move_ = NextMove::Right;
}

void Player::move_left()
{
    next_move_ = NextMove::Left;
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

bool Player::target_is_hit(PositionD const& target) const
{
    return std::visit(
        overloaded{
            [&target](SlashAttackMechanic const& attack) -> bool {
                return attack.target_is_hit(target);
            },
            [&target](ShootAttackMechanic const& attack) -> bool {
                return std::ranges::any_of(
                    attack.projectiles(),
                    [&target](PositionD const& projectile) -> bool {
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

namespace {

bool is_obstructed(
    PositionDAndOrientation const& new_value,
    std::span<Tile const> const environment)
{
    return std::ranges::any_of(
        environment, [&new_value](Tile const& tile) -> bool {
            if (tile.type == TileType::Obstacle) {
                return is_within_distance<player_size>(
                    new_value.position, to_position_d(tile.position));
            }
            return false;
        });
}

} // namespace

void Player::advance(
    std::span<Tile const> const environment,
    std::chrono::milliseconds const diff)
{
    auto const new_position =
        [this, diff]() -> std::optional<PositionDAndOrientation> {
        auto const& position = position_and_orientation_.position;
        auto const step =
            player_move_speed * gsl::narrow_cast<double>(diff.count());
        switch (next_move_) {
            case NextMove::Up:
                return PositionDAndOrientation{
                    {position.x, position.y + step}, Orientation::Up};
            case NextMove::Down:
                return PositionDAndOrientation{
                    {position.x, position.y - step}, Orientation::Down};
            case NextMove::Right:
                return PositionDAndOrientation{
                    {position.x + step, position.y}, Orientation::Right};
            case NextMove::Left:
                return PositionDAndOrientation{
                    {position.x - step, position.y}, Orientation::Left};
            default:
                break;
        };
        return std::nullopt;
    }();
    if (new_position.has_value() &&
        !is_obstructed(*new_position, environment)) {
        position_and_orientation_ = *new_position;
    }
    std::visit(
        overloaded{
            [](SlashAttackMechanic& attack) { attack.tick(); },
            [diff](ShootAttackMechanic& attack) { attack.tick(diff); },
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