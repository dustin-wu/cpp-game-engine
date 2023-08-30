#pragma once

#include "Engine/Components/BehaviorTree/condition.h"
#include "Engine/gameobject.h"

namespace ExtremeUnaliving {

class ConditionPlayerIsClose : public Condition
{
public:
    ConditionPlayerIsClose(std::shared_ptr<GameObject> entity, std::shared_ptr<GameObject> target, float dist);
    Status check(float deltaTime) override;
    void reset() override {}

private:
    std::shared_ptr<GameObject> m_entity;
    std::shared_ptr<GameObject> m_target;
    float m_dist;
};

}

