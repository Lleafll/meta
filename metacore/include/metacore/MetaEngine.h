#pragma once

#include <memory>
namespace metacore {
struct GameState;
} // namespace metacore

namespace metacore {

class MetaEngine final {
  public:
    MetaEngine();
    explicit MetaEngine(GameState const& state);
    ~MetaEngine();

    [[nodiscard]] GameState calculate_state() const;
    void input_right();
    void input_left();
    void input_up();
    void input_down();

  private:
    struct Impl;

    std::unique_ptr<Impl> impl_;
};

}; // namespace metacore