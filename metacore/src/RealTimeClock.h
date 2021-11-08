#pragma once

#include <thread>
namespace metacore {
class InternalGameState;
} // namespace metacore

namespace metacore {

class RealTimeClock final {
  public:
    explicit RealTimeClock(InternalGameState& state);

  private:
    std::jthread thread_;
};

} // namespace metacore
