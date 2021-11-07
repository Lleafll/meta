#pragma once

#include "Clock.h"
#include "DefaultState.h"
#include "GameOverState.h"
#include "PickingUpState.h"
#include <variant>

namespace metacore {

struct InternalGameState final {
    std::variant<DefaultState, PickingUpState, GameOverState> value;
};

void advance(InternalGameState& state, std::chrono::microseconds diff);

} // namespace metacore