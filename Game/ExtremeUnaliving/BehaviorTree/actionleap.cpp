#include "actionleap.h"
#include "Engine/Components/rigidbodycomponent.h"
#include "Engine/Components/transformcomponent.h"

namespace ExtremeUnaliving {

ActionLeap::ActionLeap(std::shared_ptr<GameObject> entity, std::shared_ptr<GameObject> target, std::shared_ptr<HealthSystem> healthSystem,
                         std::shared_ptr<AnimatedModel> animModel, std::shared_ptr<Animation> animation, std::shared_ptr<CollisionSystem> collisionSystem)
{
    m_entity = entity;
    m_target = target;
    windUp = 1.5f;
    coolDown = 4.f;
    maxWindUp = 1.5f;
    maxCoolDown = 4.f;
    m_healthSystem = healthSystem;
    m_animModel = animModel;
    m_animation = animation;
    launched = false;
    m_collisionSystem = collisionSystem;
}

Status ActionLeap::act(float deltaTime) {
    if (coolDown > 0) {
        coolDown -= deltaTime;
        return Status::FAILURE;
    }

    std::shared_ptr<ModelTransform> entityModel = m_entity->getComponent<TransformComponent>()->getGlobalTransform();
    std::shared_ptr<ModelTransform> targetModel = m_target->getComponent<TransformComponent>()->getGlobalTransform();
    glm::vec3 target2D = glm::vec3(targetModel->getPos().x, 0, targetModel->getPos().z);
    glm::vec3 entity2D = glm::vec3(entityModel->getPos().x, 0, entityModel->getPos().z);
    glm::vec3 dir = glm::normalize(target2D - entity2D);
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

    m_animModel->doAnimation(m_animation, maxWindUp - windUp);


    if (windUp > 0) {
        windUp -= deltaTime;
        return Status::RUNNING;
    }

    if (!launched) {
        target2D -= dir * 0.2f;
        m_entity->getComponent<RigidBodyComponent>()->setVelocity((target2D - entity2D) * 1.0f + glm::vec3(0, 5, 0));
        launched = true;
        return Status::RUNNING;
    }


    if (launched && m_collisionSystem->getCollisionBelowMap()[m_entity->ID()]) {
        m_entity->getComponent<RigidBodyComponent>()->setVelocity(glm::vec3(0, 0, 0));
        if (glm::distance(entityModel->getPos(), targetModel->getPos()) <= 5) {
            m_target->getComponent<RigidBodyComponent>()->setVelocity( m_target->getComponent<RigidBodyComponent>()->getVelocity() + glm::vec3(0, 10, 0));
            m_healthSystem->takeDamage(20);
        }
        launched = false;
        coolDown = maxCoolDown;
        windUp = maxWindUp;
        return Status::SUCCESS;
    }

    if (launched) {
        return Status::RUNNING;
    }


    // shouldnt get here
    return Status::FAILURE;







}

}
