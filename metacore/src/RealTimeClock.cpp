#include "RealTimeClock.h"
#include "InternalGameState.h"

namespace metacore {

namespace {

constexpr auto sleep_duration = std::chrono::milliseconds{5};

} // namespace

RealTimeClock::RealTimeClock(InternalGameState& state)
    : thread_{
          [](std::stop_token stop_token, InternalGameState* const state) {
              auto start = std::chrono::steady_clock::now();
              while (true) {
                  if (stop_token.stop_requested()) {
                      return;
                  }
                  std::this_thread::sleep_for(sleep_duration);
                  auto const now = std::chrono::steady_clock::now();
                  auto const diff =
                      std::chrono::duration_cast<std::chrono::microseconds>(
                          now - start);
                  start = now;
                  state->advance(diff);
              }
          },
          &state}
{
}

} // namespace metacore