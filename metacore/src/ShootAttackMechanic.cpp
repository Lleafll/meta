#include "ShootAttackMechanic.h"

namespace metacore {

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
}

} // namespace metacore
