#include "OpenWorldLayoutMechanic.h"

namespace metacore {

namespace {

constexpr auto left_bound = -375;
constexpr auto right_bound = 375;
constexpr auto bottom_bound = -275;
constexpr auto top_bound = 275;
constexpr auto width = right_bound - left_bound;
constexpr auto height = top_bound - bottom_bound;

} // namespace

bool OpenWorldLayoutMechanic::check_for_transition(Player& player)
{
    auto& position = player.position();
    if (position.x < left_bound) {
        position.x += width;
        return true;
    }
    if (position.x > right_bound) {
        position.x -= width;
        return true;
    }
    if (position.y < bottom_bound) {
        position.y += height;
        return true;
    }
    if (position.y > right_bound) {
        position.y -= height;
        return true;
    }
    return false;
}

} // namespace metacore