#include "Enemies.h"

namespace metacore {

namespace {

constexpr auto enemy_movement_steps = 25;

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

} // namespace

void Enemies::advance(Position const& player_position)
{
    for (auto& enemy : positions_) {
        move_enemy(enemy, player_position);
    }
}

void Enemies::kill(std::size_t const index)
{
    positions_.erase(positions_.begin() + index);
}

} // namespace metacore