#pragma once

#include <functional>
namespace metacore {
struct InternalGameState;
} // namespace metacore

namespace metacore {

class Clock {
  public:
    explicit Clock(InternalGameState& state);

    void input();

  private:
    std::reference_wrapper<InternalGameState> state_;
};

} // namespace metacore
