#pragma once

#include "Position.h"
#include <vector>

namespace metacore {

class Enemies final {
  public:
    Enemies() = default;
    explicit Enemies(std::vector<Position> positions);

    [[nodiscard]] std::vector<Position> const& positions() const;

    void advance(Position const& player_position);

  private:
    std::vector<Position> positions_ = {};
};

} // namespace metacore