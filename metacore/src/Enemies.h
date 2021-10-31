#pragma once

#include "Position.h"
#include <random>
#include <vector>
#include "CharacterTexture.h"

namespace metacore {

class Enemies final {
  public:
    CharacterTexture texture = CharacterTexture::None;

    Enemies() = default;
    explicit Enemies(std::vector<Position> positions);

    [[nodiscard]] std::vector<Position> const& positions() const;
    [[nodiscard]] std::vector<Position>& positions();
    void advance(Position const& player_position);
    void kill(std::size_t index);

  private:
    std::vector<Position> positions_ = {};
    int spawn_counter_ = 0;
    std::mt19937 random_engine_ = std::mt19937{std::random_device{}()};
};

} // namespace metacore