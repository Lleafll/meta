#pragma once

namespace metacore {

class SlashAttackMechanic final {
  public:
    [[nodiscard]] bool is_active() const;
    void start();
    void tick();

  private:
    bool is_active_ = false;
};

} // namespace metacore