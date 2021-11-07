#include "SlashAttackMechanic.h"

namespace metacore {

namespace {

constexpr auto slash_radius = 50.0;
constexpr auto attack_duration = std::chrono::microseconds{200'000};

} // namespace

bool SlashAttackMechanic::is_active() const
{
    return time_passed_ < attack_duration;
}

void SlashAttackMechanic::start(PositionDAndOrientation const& where)
{
    time_passed_ = {};
    where_ = where;
}

void SlashAttackMechanic::tick(std::chrono::microseconds const diff)
{
    time_passed_ += diff;
}

bool SlashAttackMechanic::target_is_hit(PositionD const& target) const
{
    if (!is_active()) {
        return false;
    }
    auto const& position = where_.position;
    if (!is_within_distance<slash_radius>(position, target)) {
        return false;
    }
    switch (where_.orientation) {
        case Orientation::Up:
            return target.y > position.y;
        case Orientation::Down:
            return target.y < position.y;
        case Orientation::Left:
            return target.x < position.x;
        case Orientation::Right:
            return target.x > position.x;
    }
    return false;
}

} // namespace metacore