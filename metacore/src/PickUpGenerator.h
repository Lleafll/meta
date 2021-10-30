#pragma once

#include "LayoutBounds.h"
#include <optional>
#include <random>
namespace metacore {
struct LayoutBounds;
struct Pickup;
} // namespace metacore

namespace metacore {

class PickUpGenerator final {
  public:
    Pickup generate(std::optional<LayoutBounds> const& bounds);

  private:
    std::mt19937 random_engine_ = std::mt19937{std::random_device{}()};
};

} // namespace metacore