#include "healthsystem.h"
#include "Engine/Components/nocomponent.h"

namespace PrairieKing3D {

HealthSystem::HealthSystem()
    : System(typeid(NoComponent))
{
    reset();
}

void HealthSystem::reset() {
    health_ = 10;
}

void HealthSystem::takeDamage(int damage) {
    health_ -= damage;
    health_ = std::max(0, health_);
}

std::string HealthSystem::getHealthInfo() {
    return "Health: " + std::to_string(health_);
}


}
