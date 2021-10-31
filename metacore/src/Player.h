#pragma once

#include "CharacterTexture.h"
#include "Position.h"
#include "PositionAndOrientation.h"
#include "ShootAttackMechanic.h"
#include "SlashAttackMechanic.h"
#include "Tile.h"
#include <span>
#include <variant>
#include <vector>

namespace metacore {

enum class AttackUpgrade { Slash, Shoot };

class Player final {
  public:
    CharacterTexture texture = CharacterTexture::None;

    Player() = default;
    explicit Player(Position const& position);
    explicit Player(PositionAndOrientation const& position_and_orientation);

    [[nodiscard]] Position const& position() const;
    [[nodiscard]] Position& position();
    [[nodiscard]] Orientation orientation() const;
    [[nodiscard]] bool is_slashing() const;
    [[nodiscard]] std::vector<Position> const* projectiles() const;
    void move_up(std::span<Tile const> environment);
    void move_down(std::span<Tile const> environment);
    void move_right(std::span<Tile const> environment);
    void move_left(std::span<Tile const> environment);
    void attack();
    void set_attack(AttackUpgrade upgrade);
    [[nodiscard]] bool target_is_hit(Position const& target) const;

  private:
    PositionAndOrientation position_and_orientation_ =
        PositionAndOrientation{{0, 0}, Orientation::Up};
    std::variant<std::monostate, SlashAttackMechanic, ShootAttackMechanic>
        attack_ = {};
};

} // namespace metacore