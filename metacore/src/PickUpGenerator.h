#pragma once

#include <random>
namespace metacore {
struct Pickup;
} // namespace metacore

namespace metacore {

class PickUpGenerator final {
  public:
    Pickup generate();

  private:
    std::mt19937 random_engine_ = std::mt19937{std::random_device{}()};
};

} // namespace metacore