#include "enemycomponent.h"
#include <cmath>

EnemyComponent::EnemyComponent(GameObjectID targetID)
    : enemySpeed_(1)
    , enemyTurnSpeed_(0.5)
    , targetID_(targetID)
    , attackCooldown_(0)
{

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
