#pragma once

#include "Position.h"
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
    Position position_ = Position{0, 0};
    std::variant<std::monostate, SlashAttackMechanic, ShootAttackMechanic>
        attack_ = {};
};

} // namespace metacore