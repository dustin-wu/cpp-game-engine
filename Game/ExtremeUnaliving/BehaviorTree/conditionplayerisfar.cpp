#include "conditionplayerisfar.h"
#include "Engine/Components/transformcomponent.h"
#include "glm/geometric.hpp"

namespace ExtremeUnaliving {

ConditionPlayerIsFar::ConditionPlayerIsFar(std::shared_ptr<GameObject> entity, std::shared_ptr<GameObject> target, float dist, std::shared_ptr<CollisionSystem> collisionSystem, bool rayCheck)
{
    m_entity = entity;
    m_target = target;
    m_dist = dist;
    m_collisionSystem = collisionSystem;
    rayCheck_ = rayCheck;
}

Status ConditionPlayerIsFar::check(float deltaTime) {
    std::shared_ptr<ModelTransform> entityModel = m_entity->getComponent<TransformComponent>()->getGlobalTransform();
    std::shared_ptr<ModelTransform> targetModel = m_target->getComponent<TransformComponent>()->getGlobalTransform();
    bool playerTooFar = (glm::distance(entityModel->getPos(), targetModel->getPos()) >= m_dist);

    glm::vec3 rayStart = entityModel->getPos() + glm::vec3(0, 2, 0);
    glm::vec3 rayDir = glm::normalize(targetModel->getPos() - rayStart);
    float hitEnvT = m_collisionSystem->rayTraceTriangles(rayStart, rayDir);
    GameObjectID closestHit;
    float hitPlayerT = m_collisionSystem->checkRayHit(m_target->ID(), rayStart, rayDir);
    bool playerOnGround = m_collisionSystem->getCollisionBelowMap()[m_target->ID()];
    bool cantSeePlayer = (hitEnvT < hitPlayerT) && (playerOnGround);

    if (!rayCheck_) {
        if (playerTooFar && !cantSeePlayer) {
            return Status::SUCCESS;
        } else {
            return Status::FAILURE;
        }
    }

    if (playerTooFar || cantSeePlayer) {
        return Status::SUCCESS;
    }

    return Status::FAILURE;

}




}
