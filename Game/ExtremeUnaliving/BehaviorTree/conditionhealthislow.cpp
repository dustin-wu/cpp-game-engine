#include "conditionhealthislow.h"
#include "Game/ExtremeUnaliving/Components/enemycomponent.h"

namespace ExtremeUnaliving {

ConditionHealthIsLow::ConditionHealthIsLow(std::shared_ptr<GameObject> entity)
{
    m_entity = entity;
}

Status ConditionHealthIsLow::check(float deltaTime) {
    if (m_entity->getComponent<EnemyComponent>()->getHP() <= 1) {
//        std::cout << "run away condition true" << std::endl;
        return Status::SUCCESS;
    }
    return Status::FAILURE;
}

}
