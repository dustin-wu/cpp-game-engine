#include "physicssystem.h"
#include "Engine/Components/rigidbodycomponent.h"
#include "Engine/Components/transformcomponent.h"
#include "Engine/Systems/collisionsystem.h"

PhysicsSystem::PhysicsSystem()
    : System(typeid(RigidBodyComponent))
{

}

void PhysicsSystem::applyKinematics(float deltaTimef) {
    for (auto& [id, gameObject]: m_gameObjects) {
        std::shared_ptr<RigidBodyComponent> rigidBodyComponent = gameObject->getComponent<RigidBodyComponent>();
        if (rigidBodyComponent->getStationary()) {
            continue;
        }
        std::shared_ptr<TransformComponent> transformComponent = gameObject->getComponent<TransformComponent>();


        glm::vec3 currentPos = transformComponent->getGlobalTransform()->getPos();
        glm::vec3 currentVelocity = rigidBodyComponent->getVelocity();
        glm::vec3 currentAcceleration = rigidBodyComponent->getAcceleration();

        if (rigidBodyComponent->getAffectedByGravity()) {
            currentAcceleration += glm::vec3(0, -rigidBodyComponent->getGravity(), 0);
        }


        bool onGround = transformComponent->getGlobalTransform()->getPos().y <= 0;
        bool collisionBelow = m_gameWorld.lock()->getSystem<CollisionSystem>()->getCollisionBelowMap()[id];
        if ((onGround || collisionBelow) && currentVelocity.y < 0) {
            rigidBodyComponent->setVelocity(glm::vec3(currentVelocity.x, 0, currentVelocity.z));
            currentVelocity = rigidBodyComponent->getVelocity();
        }

        glm::vec3 dPos = (deltaTimef * currentVelocity) + (0.5f * currentAcceleration * deltaTimef * deltaTimef);
        transformComponent->getGlobalTransform()->translate(dPos); // x += v * dt + 1/2 * a * dt^2
        currentPos = transformComponent->getGlobalTransform()->getPos();

        onGround = transformComponent->getGlobalTransform()->getPos().y <= 0;
        if (onGround) {
            transformComponent->getGlobalTransform()->setPos(glm::vec3(currentPos.x, 0, currentPos.z));
        }



        glm::vec3 dV = currentAcceleration * deltaTimef;
        rigidBodyComponent->setVelocity(currentVelocity + dV); // v += g * dt
    }
}


