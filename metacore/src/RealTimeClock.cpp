#include "RealTimeClock.h"
#include "InternalGameState.h"

namespace metacore {

namespace {

constexpr auto sleep_duration = std::chrono::milliseconds{50};

} // namespace

RealTimeClock::RealTimeClock(InternalGameState& state)
    : thread_{
          [](std::stop_token stop_token, InternalGameState* const state) {
              while (true) {
                  if (stop_token.stop_requested()) {
                      return;
                  }
                  advance(*state);
                  std::this_thread::sleep_for(sleep_duration);
              }
          },
          &state}
{
}

} // namespace metacore