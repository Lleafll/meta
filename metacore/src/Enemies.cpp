#include "Enemies.h"
#include <gsl/gsl_assert>
#include <gsl/gsl_util>

namespace metacore {

namespace {

constexpr auto enemy_movement_steps = 25;
constexpr auto spawn_interval = 4;
constexpr auto spawn_edge = 300;

} // namespace

Enemies::Enemies(std::vector<Position> positions)
    : positions_{std::move(positions)}
{
}

std::vector<Position> const& Enemies::positions() const
{
    return positions_;
}

namespace {

enum class Direction { None, Up, Down, Right, Left };

Direction
decide_enemy_movement_direction(Position const& enemy, Position const& target)
{
    auto const x_diff = target.x - enemy.x;
    auto const y_diff = target.y - enemy.y;
    auto const abs_x_diff = std::abs(x_diff);
    auto const abs_y_diff = std::abs(y_diff);
    if (abs_x_diff < enemy_movement_steps &&
        abs_y_diff < enemy_movement_steps) {
        return Direction::None;
    }
    if (std::abs(x_diff) > std::abs(y_diff)) {
        if (x_diff > 0) {
            return Direction::Right;
        } else {
            return Direction::Left;
        }
    } else {
        if (y_diff > 0) {
            return Direction::Up;
        } else {
            return Direction::Down;
        }
    }
}

void move_enemy(Position& enemy, Position const& target)
{
    switch (decide_enemy_movement_direction(enemy, target)) {
        case Direction::Up:
            enemy.y += enemy_movement_steps;
            break;
        case Direction::Down:
            enemy.y -= enemy_movement_steps;
            break;
        case Direction::Right:
            enemy.x += enemy_movement_steps;
            break;
        case Direction::Left:
            enemy.x -= enemy_movement_steps;
            break;
        default:
            break;
    }
}

Position generate_random_position(std::mt19937& random_engine)
{
    auto const side = std::uniform_int_distribution<int>{0, 3}(random_engine);
    auto const position_on_edge = std::uniform_int_distribution<int>{
        -spawn_edge, spawn_edge}(random_engine);
    switch (side) {
        case 0:
            return Position{-spawn_edge, position_on_edge};
        case 1:
            return Position{spawn_edge, position_on_edge};
        case 2:
            return Position{-position_on_edge, -spawn_edge};
        case 3:
            return Position{-position_on_edge, spawn_edge};
        case 4:
        default:
            break;
    }
    Expects(side >= 0 && side <= 3);
    abort();
}

} // namespace

void Enemies::advance(Position const& player_position)
{
    for (auto& enemy : positions_) {
        move_enemy(enemy, player_position);
    }
    ++spawn_counter_;
    if (spawn_counter_ == spawn_interval) {
        positions_.push_back(generate_random_position(random_engine_));
        spawn_counter_ = 0;
    }
}

void Enemies::kill(std::size_t const index)
{
    positions_.erase(positions_.begin() + gsl::narrow_cast<long long>(index));
}

} // namespace metacore