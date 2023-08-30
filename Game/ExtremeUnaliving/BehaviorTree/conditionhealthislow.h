#pragma once

#include "Engine/Components/BehaviorTree/condition.h"
#include "Engine/gameobject.h"

namespace ExtremeUnaliving {


class ConditionHealthIsLow : public Condition
{
public:
    ConditionHealthIsLow(std::shared_ptr<GameObject> entity);
    Status check(float deltaTime) override;
    void reset() override {}
private:
    std::shared_ptr<GameObject> m_entity;
};

}
