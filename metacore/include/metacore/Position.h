#pragma once

namespace metacore {

struct Position final {
    int x = 0;
    int y = 0;

    friend bool operator==(Position const&, Position const&) = default;
};

namespace detail {

template<int distance>
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

template<int distance>
constexpr bool is_within_distance(Position const& lhs, Position const& rhs)
{
    return detail::is_within_distance<distance>(lhs.x, rhs.x) &&
        detail::is_within_distance<distance>(lhs.y, lhs.y);
}

} // namespace metacore
