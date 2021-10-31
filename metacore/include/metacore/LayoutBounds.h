#pragma once

namespace metacore {

struct LayoutBounds final {
    int left;
    int right;
    int bottom;
    int top;

    bool operator==(LayoutBounds const&) const = default;
};

} // namespace metacore