#pragma once

#include <memory>
namespace metacore {
struct GameState;
class InternalGameState;
} // namespace metacore

namespace metacore {

class MetaEngine final {
  public:
    struct Impl;

    MetaEngine();
    explicit MetaEngine(InternalGameState state);
    ~MetaEngine();

    [[nodiscard]] GameState calculate_state() const;
    void input_right();
    void input_left();
    void input_up();
    void input_down();
    void input_attack();
    void input_restart();
    void input_stop();
    void select_first_upgrade();
    void select_second_upgrade();

  private:
    std::unique_ptr<Impl> impl_;
};

}; // namespace metacore