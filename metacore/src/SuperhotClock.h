#pragma once

#include <thread>
namespace metacore {
class InternalGameState;
} // namespace metacore

namespace metacore {

class SuperhotClock final {
  public:
    explicit SuperhotClock(InternalGameState& state);

    void input_start();
    void input_stop();

  private:
    std::unique_ptr<bool> input_ = std::make_unique<bool>(false);
    std::jthread thread_;
};

} // namespace metacore
