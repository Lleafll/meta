#pragma once

namespace metacore {

struct Position final {
    int x = 0;
    int y = 0;

    bool operator==(Position const &) const = default;
};

} // namespace metacore
