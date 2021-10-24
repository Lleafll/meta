#pragma once

#include "Position.h"
#include <vector>

namespace metacore {

class Enemies final {
  public:
    explicit Enemies(std::vector<Position> positions);

    [[nodiscard]] std::vector<Position> const& positions() const;

  private:
    std::vector<Position> positions_ = {};
};

} // namespace metacore