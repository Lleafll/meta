#pragma once

namespace metacore {

struct Position final {
    double x;
    double y;

    bool operator==(Position const &) const = default;
};

} // namespace metacore
