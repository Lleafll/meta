#include "Enemies.h"
#include <gsl/gsl_assert>
#include <gsl/gsl_util>

namespace metacore {

namespace {

constexpr auto enemy_movement_speed = 10.0 / 1'000'000.0;
constexpr auto spawn_interval = std::chrono::microseconds{4'000'000};
constexpr auto spawn_edge = 300;

} // namespace

Enemies::Enemies(std::vector<PositionD> positions)
    : positions_{std::move(positions)}
{
}

std::vector<PositionD> const& Enemies::positions() const
{
    return positions_;
}

std::vector<PositionD>& Enemies::positions()
{
    return positions_;
}

namespace {

enum class Direction { None, Up, Down, Right, Left };

Direction decide_enemy_movement_direction(
    PositionD const& enemy, PositionD const& target, double const step)
{
    auto const x_diff = target.x - enemy.x;
    auto const y_diff = target.y - enemy.y;
    auto const abs_x_diff = std::abs(x_diff);
    auto const abs_y_diff = std::abs(y_diff);
    if (abs_x_diff < step && abs_y_diff < step) {
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

void move_enemy(PositionD& enemy, PositionD const& target, double const step)
{
    switch (decide_enemy_movement_direction(enemy, target, step)) {
        case Direction::Up:
            enemy.y += step;
            break;
        case Direction::Down:
            enemy.y -= step;
            break;
        case Direction::Right:
            enemy.x += step;
            break;
        case Direction::Left:
            enemy.x -= step;
            break;
        default:
            break;
    }
}

PositionD generate_random_position(std::mt19937& random_engine)
{
    auto const side = std::uniform_int_distribution<int>{0, 3}(random_engine);
    auto const position_on_edge = std::uniform_real_distribution<double>{
        -spawn_edge, spawn_edge}(random_engine);
    switch (side) {
        case 0:
            return PositionD{-spawn_edge, position_on_edge};
        case 1:
            return PositionD{spawn_edge, position_on_edge};
        case 2:
            return PositionD{-position_on_edge, -spawn_edge};
        case 3:
            return PositionD{-position_on_edge, spawn_edge};
        case 4:
        default:
            break;
    }
    Expects(side >= 0 && side <= 3);
    abort();
}

} // namespace

void Enemies::advance(
    PositionD const& player_position, std::chrono::microseconds const diff)
{
    auto const step =
        enemy_movement_speed * gsl::narrow_cast<double>(diff.count());
    for (auto& enemy : positions_) {
        move_enemy(enemy, player_position, step);
    }
    spawn_counter_ += diff;
    if (spawn_counter_ >= spawn_interval) {
        positions_.push_back(generate_random_position(random_engine_));
        spawn_counter_ = {};
    }
}

void Enemies::kill(std::size_t const index)
{
    positions_.erase(positions_.begin() + gsl::narrow_cast<long long>(index));
}

} // namespace metacore