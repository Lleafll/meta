#include "MetaEngine.h"
#include "Clock.h"
#include "DefaultState.h"
#include "GameState.h"
#include "InternalGameState.h"

namespace metacore {

struct MetaEngine::Impl final {
    InternalGameState state_;
};

MetaEngine::MetaEngine() : impl_{std::make_unique<Impl>()}
{
}

MetaEngine::MetaEngine(InternalGameState state)
    : impl_{std::make_unique<Impl>(Impl{std::move(state)})}
{
}

MetaEngine::~MetaEngine() = default;

GameState MetaEngine::calculate_state() const
{
    return impl_->state_.to_game_state();
}

void MetaEngine::input_right()
{
    impl_->state_.move_player(Orientation::Right);
}

void MetaEngine::input_left()
{
    impl_->state_.move_player(Orientation::Left);
}

void MetaEngine::input_up()
{
    impl_->state_.move_player(Orientation::Up);
}

void MetaEngine::input_down()
{
    impl_->state_.move_player(Orientation::Down);
}

void MetaEngine::input_attack()
{
    impl_->state_.attack_player();
}

void MetaEngine::input_restart()
{
    impl_ = std::make_unique<Impl>();
}

void MetaEngine::input_stop()
{
    impl_->state_.stop_player();
}

void MetaEngine::select_first_upgrade()
{
    impl_->state_.select_upgrade(UpgradeSelection::First);
}

void MetaEngine::select_second_upgrade()
{
    impl_->state_.select_upgrade(UpgradeSelection::Second);
}

} // namespace metacore
