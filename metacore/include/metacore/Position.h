#pragma once

namespace metacore {

struct Position final {
    int x = 0;
    int y = 0;

    friend bool operator==(Position const &, Position const &) = default;
};

} // namespace metacore
