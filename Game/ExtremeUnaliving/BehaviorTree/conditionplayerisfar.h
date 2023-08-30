#pragma once


#include "Engine/Components/BehaviorTree/condition.h"
#include "Engine/Systems/collisionsystem.h"
#include "Engine/gameobject.h"

namespace ExtremeUnaliving {

class ConditionPlayerIsFar : public Condition
{
public:
    ConditionPlayerIsFar(std::shared_ptr<GameObject> entity, std::shared_ptr<GameObject> target, float dist, std::shared_ptr<CollisionSystem> collisionSystem, bool rayCheck = true);
    Status check(float deltaTime) override;
    void reset() override {}

private:
    std::shared_ptr<GameObject> m_entity;
    std::shared_ptr<GameObject> m_target;
    std::shared_ptr<CollisionSystem> m_collisionSystem;
    float m_dist;
    bool rayCheck_;
};

}

