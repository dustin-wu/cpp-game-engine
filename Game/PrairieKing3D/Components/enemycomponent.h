#include "Engine/gameobject.h"
#include <Engine/component.h>

#pragma once

class EnemyComponent : public Component
{
public:
    EnemyComponent(GameObjectID targetID);
    float getSpeed();
    float getTurnSpeed();
    void setAttackCooldown(float cooldown) {attackCooldown_ = cooldown;}
    float getAttackCooldown() {return attackCooldown_;}
    GameObjectID getTarget();

private:
    float enemySpeed_;
    float enemyTurnSpeed_;
    float attackCooldown_;
    GameObjectID targetID_;
};
