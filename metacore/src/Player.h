#pragma once

#include "CharacterTexture.h"
#include "PositionD.h"
#include "PositionDAndOrientation.h"
#include "ShootAttackMechanic.h"
#include "SlashAttackMechanic.h"
#include "Tile.h"
#include <chrono>
#include <optional>
#include <span>
#include <variant>
#include <vector>

namespace metacore {

enum class AttackUpgrade { Slash, Shoot };

class Player final {
  public:
    CharacterTexture texture = CharacterTexture::None;

    Player() = default;
    explicit Player(PositionD const& position);
    explicit Player(PositionDAndOrientation const& position_and_orientation);

    [[nodiscard]] PositionD const& position() const;
    [[nodiscard]] PositionD& position();
    [[nodiscard]] Orientation orientation() const;
    [[nodiscard]] bool is_slashing() const;
    [[nodiscard]] std::vector<PositionD> const* projectiles() const;
    void move_up();
    void move_down();
    void move_right();
    void move_left();
    void stop();
    void attack();
    void set_attack(AttackUpgrade upgrade);
    [[nodiscard]] bool target_is_hit(PositionD const& target) const;
    void advance(
        std::span<Tile const> environment,
        std::chrono::milliseconds diff = std::chrono::milliseconds{1000});

  private:
    enum class NextMove { None, Up, Down, Right, Left, Attack };
    NextMove next_move_ = NextMove::None;
    PositionDAndOrientation position_and_orientation_ =
        PositionDAndOrientation{{0, 0}, Orientation::Up};
    std::variant<std::monostate, SlashAttackMechanic, ShootAttackMechanic>
        attack_ = {};
};

} // namespace metacore