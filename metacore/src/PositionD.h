#pragma once

#include "Position.h"
#include <gsl/gsl_util>

namespace metacore {

struct PositionD final {
    double x;
    double y;

    bool operator==(PositionD const&) const = default;
};

constexpr PositionD to_position_d(Position const& position)
{
    return {
        gsl::narrow_cast<double>(position.x),
        gsl::narrow_cast<double>(position.y)};
}

constexpr Position to_position(PositionD const& position_d)
{
    return {
        gsl::narrow_cast<int>(position_d.x),
        gsl::narrow_cast<int>(position_d.y)};
}

namespace detail {

template<double distance>
constexpr bool is_within_distance(double const lhs, double const rhs)
{
    auto const diff = lhs - rhs;
    if (diff >= 0) {
        return diff <= distance;
    } else {
        return diff >= -distance;
    }
}

} // namespace detail

template<double distance>
constexpr bool is_within_distance(PositionD const& lhs, PositionD const& rhs)
{
    return detail::is_within_distance<distance>(lhs.x, rhs.x) &&
        detail::is_within_distance<distance>(lhs.y, rhs.y);
}

} // namespace metacore