#pragma once

#include "Position.h"
#include "PositionAndOrientation.h"
#include "ShootAttackMechanic.h"
#include "SlashAttackMechanic.h"
#include <variant>
#include <vector>

namespace metacore {

enum class AttackUpgrade { Slash, Shoot };

class Player final {
  public:
    Player() = default;
    explicit Player(Position const& position);
    explicit Player(PositionAndOrientation const& position_and_orientation);

    [[nodiscard]] Position const& position() const;
    [[nodiscard]] bool is_slashing() const;
    [[nodiscard]] std::vector<Position> const* projectiles() const;
    void move_up();
    void move_down();
    void move_right();
    void move_left();
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