#include "SlashAttackMechanic.h"

namespace metacore {

namespace {

constexpr auto slash_radius = 50.0;

} // namespace

bool SlashAttackMechanic::is_active() const
{
    return is_active_;
}

void SlashAttackMechanic::start(PositionDAndOrientation const& where)
{
    is_active_ = true;
    where_ = where;
}

void SlashAttackMechanic::tick()
{
    is_active_ = false;
}

bool SlashAttackMechanic::target_is_hit(PositionD const& target) const
{
    if (!is_active_) {
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