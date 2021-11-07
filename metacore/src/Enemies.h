#pragma once

#include "CharacterTexture.h"
#include "PositionD.h"
#include <chrono>
#include <random>
#include <vector>

namespace metacore {

class Enemies final {
  public:
    CharacterTexture texture = CharacterTexture::None;

    Enemies() = default;
    explicit Enemies(std::vector<PositionD> positions);

    [[nodiscard]] std::vector<PositionD> const& positions() const;
    [[nodiscard]] std::vector<PositionD>& positions();
    void advance(
        PositionD const& player_position,
        std::chrono::microseconds diff = std::chrono::microseconds{5'000'000});
    void kill(std::size_t index);

  private:
    std::vector<PositionD> positions_ = {};
    std::chrono::microseconds spawn_counter_ = {};
    std::mt19937 random_engine_ = std::mt19937{std::random_device{}()};
};

} // namespace metacore