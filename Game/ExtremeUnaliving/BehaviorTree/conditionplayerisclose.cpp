#include "conditionplayerisclose.h"
#include "Engine/Components/transformcomponent.h"

namespace ExtremeUnaliving {

ConditionPlayerIsClose::ConditionPlayerIsClose(std::shared_ptr<GameObject> entity, std::shared_ptr<GameObject> target, float dist)
{
    m_entity = entity;
    m_target = target;
    m_dist = dist;
}

Status ConditionPlayerIsClose::check(float deltaTime) {
    std::shared_ptr<ModelTransform> entityModel = m_entity->getComponent<TransformComponent>()->getGlobalTransform();
    std::shared_ptr<ModelTransform> targetModel = m_target->getComponent<TransformComponent>()->getGlobalTransform();

    if (glm::distance(entityModel->getPos(), targetModel->getPos()) <= m_dist) {
        return Status::SUCCESS;
    }
    return Status::FAILURE;
}

}
