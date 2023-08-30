#include "healthsystem.h"
#include "Engine/Components/nocomponent.h"
#include "Game/ExtremeUnaliving/ShopHandler.h"

namespace ExtremeUnaliving {

HealthSystem::HealthSystem()
    : System(typeid(NoComponent))
{
    reset();
    coolDown = 0;
}

void HealthSystem::reset() {
    health_ = 100;
}

void HealthSystem::takeDamage(int damage) {
    if (ShopHandling::handler.itemBought(5)) {
        damage -= 2;
        damage = std::max(0, damage);
    }
    health_ -= damage;
    health_ = std::max(0, health_);
}

int HealthSystem::getPlayerHealth() {
    return health_;
}

void HealthSystem::update(float deltaTime) {
    if (ShopHandling::handler.itemBought(1)) {
        if (coolDown <= 0.f) {
            health_ = std::min(health_ + 5, 100);
            coolDown = 10.f;
        } else {
            coolDown -= deltaTime;
        }
    }
}




}
