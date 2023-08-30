#include "enemycomponent.h"
#include <cmath>

namespace ExtremeUnaliving {

EnemyComponent::EnemyComponent(GameObjectID targetID)
    : enemySpeed_(7.f)
    , enemyTurnSpeed_(2.f)
    , targetID_(targetID)
    , attackCooldown_(0)

{
    healthPoints_ = HEALTHPOINTSMAX;
}

float EnemyComponent::getSpeed() {
    return enemySpeed_;
}

float EnemyComponent::getTurnSpeed() {
    return enemyTurnSpeed_;
}

GameObjectID EnemyComponent::getTarget() {
    return targetID_;
}

float EnemyComponent::getHP() {
    return healthPoints_;
}

void EnemyComponent::setHP(float hp) {
    healthPoints_ = hp;
}

float EnemyComponent::getMaxHP() {
    return HEALTHPOINTSMAX;
}

}
