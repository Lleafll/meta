#include "ShootAttackMechanic.h"

namespace metacore {

namespace {

constexpr auto projectile_step_size = 50;

} // namespace

ShootAttackMechanic::ShootAttackMechanic(std::vector<Position> projectiles)
    : projectiles_{std::move(projectiles)}
{
}

std::vector<Position> const& ShootAttackMechanic::projectiles() const
{
    return projectiles_;
}

void ShootAttackMechanic::start(Position const& position)
{
    projectiles_.push_back(position);
}

void ShootAttackMechanic::tick()
{
    for (auto& projectile : projectiles_) {
        projectile.x += projectile_step_size;
    }
}

} // namespace metacore
