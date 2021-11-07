#include "SuperhotClock.h"
#include "InternalGameState.h"

namespace metacore {

namespace {

constexpr auto sleep_duration = std::chrono::milliseconds{5};
constexpr auto slow_motion_rate = 20;

} // namespace

SuperhotClock::SuperhotClock(InternalGameState& state)
    : thread_{
          [](std::stop_token const& stop_token,
             InternalGameState* const state,
             bool const* const input) {
              auto start = std::chrono::steady_clock::now();
              while (true) {
                  if (stop_token.stop_requested()) {
                      return;
                  }
                  std::this_thread::sleep_for(sleep_duration);
                  auto const now = std::chrono::steady_clock::now();
                  auto diff =
                      std::chrono::duration_cast<std::chrono::microseconds>(
                          now - start);
                  if (!(*input)) {
                      diff /= slow_motion_rate;
                  }
                  start = now;
                  advance(*state, diff);
              }
          },
          &state,
          input_.get()}
{
}

void SuperhotClock::input_start()
{
    *input_ = true;
}

void SuperhotClock::input_stop()
{
    *input_ = false;
}

} // namespace metacore
