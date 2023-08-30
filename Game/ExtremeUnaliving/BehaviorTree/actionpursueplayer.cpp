#include "actionpursueplayer.h"
#include "Engine/Components/CollisionComponents/ellipsoidcollisioncomponent.h"
#include "Engine/Components/transformcomponent.h"
#include "Game/ExtremeUnaliving/Components/enemycomponent.h"
#include "glm/fwd.hpp"

namespace ExtremeUnaliving {

ActionPursuePlayer::ActionPursuePlayer(std::shared_ptr<GameObject> entity, std::shared_ptr<GameObject> target, std::shared_ptr<NavMeshSystem> navSystem,
                                       std::shared_ptr<AnimatedModel> animModel, std::shared_ptr<Animation> animation)
{
    m_entity = entity;
    m_target = target;
    m_navSystem = navSystem;
    pathProgress = 0;
    previousPos = m_entity->getComponent<TransformComponent>()->getGlobalTransform()->getPos();
    m_animModel = animModel;
    m_animation = animation;
}

Status ActionPursuePlayer::act(float deltaTime) {
     m_animModel->doAnimation(m_animation);

    if (findPathCooldown <= 0 || m_entity->getComponent<EnemyComponent>()->justLunged) {
        chaserPath = m_navSystem->findPath(m_entity->ID(), m_target->ID());
        pathProgress = 0;
        findPathCooldown = COOLDOWNMAX;
        m_entity->getComponent<EnemyComponent>()->justLunged = false;
    } else {
        findPathCooldown -= deltaTime;
    }

    if (chaserPath.size() == 0) return Status::FAILURE;
    glm::vec3 nextPos = chaserPath[pathProgress];
    nextPos = glm::vec3(nextPos.x, 0, nextPos.z);
    glm::vec3 curPos = m_entity->getComponent<TransformComponent>()->getGlobalTransform()->getPos();
    curPos = glm::vec3(curPos.x, 0, curPos.z);
    float radius = m_entity->getComponent<EllipsoidCollisionComponent>()->getRadii().x;
    glm::vec3 targetObjPos =  m_target->getComponent<TransformComponent>()->getGlobalTransform()->getPos();
    targetObjPos = glm::vec3(targetObjPos.x, 0, targetObjPos.z);
    if (glm::distance(curPos, targetObjPos) <= radius) return Status::SUCCESS;


    // If entity is close enough to its target or if we've been there previously, increment the path index

    if ((glm::distance(curPos, nextPos) <= radius && pathProgress < chaserPath.size() - 1)
            || glm::distance(nextPos, previousPos) <= radius) {
        pathProgress++;
        nextPos = chaserPath[pathProgress];
        previousPos = curPos;
    }

    std::shared_ptr<ModelTransform> entityModel = m_entity->getComponent<TransformComponent>()->getGlobalTransform();

    glm::vec3 dir = glm::normalize(nextPos - curPos);
    glm::vec3 up = glm::vec3(0, 1, 0);
    glm::vec3 right = glm::normalize(glm::cross(dir, up));

    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0][0] = -right.x;
    rotation[0][1] = -right.y;
    rotation[0][2] = -right.z;
    rotation[1][0] = up.x;
    rotation[1][1] = up.y;
    rotation[1][2] = up.z;
    rotation[2][0] = dir.x;
    rotation[2][1] = dir.y;
    rotation[2][2] = dir.z;

    glm::quat startRotation = entityModel->getRotation();
    glm::quat endRotation = rotation;
    float rotationSpeed = 10.0f;
    glm::quat interpolatedRotation = glm::slerp(startRotation, endRotation, deltaTime * rotationSpeed);
    entityModel->setRotation(glm::mat4(interpolatedRotation));
//    std::cout << "Enemy ID: " << m_entity->ID() << std::endl;
    entityModel->translate(deltaTime * m_entity->getComponent<EnemyComponent>()->getSpeed() * dir + glm::vec3(0, 0.01, 0));
    return Status::SUCCESS;
}

}

