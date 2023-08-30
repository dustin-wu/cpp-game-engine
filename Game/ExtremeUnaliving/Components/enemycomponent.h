#include "Engine/gameobject.h"
#include <Engine/component.h>

#pragma once

namespace ExtremeUnaliving {

class EnemyComponent : public Component
{
public:
    EnemyComponent(GameObjectID targetID);
    float getSpeed();
    float getTurnSpeed();
    void setAttackCooldown(float cooldown) {attackCooldown_ = cooldown;}
    float getAttackCooldown() {return attackCooldown_;}
    GameObjectID getTarget();
    float getHP();
    void setHP(float hp);
    float getMaxHP();
    bool justLunged;
    int enemyType;
    void setSpeed(float speed) {enemySpeed_ = speed;}
    void setMaxHP(float maxHP) {HEALTHPOINTSMAX = maxHP; healthPoints_ = HEALTHPOINTSMAX;}

private:
    float healthPoints_;
    float HEALTHPOINTSMAX = 3;
    float enemySpeed_;
    float enemyTurnSpeed_;
    float attackCooldown_;
    GameObjectID targetID_;
};


}
