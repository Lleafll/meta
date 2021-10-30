#include "OpenWorldLayoutMechanic.h"
#include <optional>

namespace metacore {

namespace {

constexpr auto left_bound = -375;
constexpr auto right_bound = 375;
constexpr auto bottom_bound = -275;
constexpr auto top_bound = 275;
constexpr auto width = right_bound - left_bound;
constexpr auto height = top_bound - bottom_bound;

std::optional<Orientation> check_if_out_of_bounds(Position const& position)
{
    if (position.x < left_bound) {
        return Orientation::Left;
    }
    if (position.x > right_bound) {
        return Orientation::Right;
    }
    if (position.y < bottom_bound) {
        return Orientation::Down;
    }
    if (position.y > right_bound) {
        return Orientation::Up;
    }
    return std::nullopt;
}

void move_from_bound(Position& position, Orientation const bound)
{
    switch (bound) {
        case Orientation::Left:
            position.x += width;
            break;
        case Orientation::Right:
            position.x -= width;
            break;
        case Orientation::Down:
            position.y += height;
            break;
        case Orientation::Up:
            position.y -= height;
            break;
    }
}

} // namespace

bool OpenWorldLayoutMechanic::check_for_transition(
    Player& player, std::span<Position> const enemies)
{
    auto const bound = check_if_out_of_bounds(player.position());
    if (bound.has_value()) {
        move_from_bound(player.position(), *bound);
        for (auto& enemy : enemies) {
            move_from_bound(enemy, *bound);
        }
        return true;
    }
    return false;
}

} // namespace metacore