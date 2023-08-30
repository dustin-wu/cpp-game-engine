#include "jumppadsystem.h"
#include "Engine/Components/playercomponent.h"
#include "Engine/Components/rigidbodycomponent.h"
#include "Engine/Components/transformcomponent.h"
#include "Engine/Systems/collisionsystem.h"
#include "Game/ExtremeUnaliving/Components/enemycomponent.h"
#include "Game/ExtremeUnaliving/Components/jumppadcomponent.h"

namespace ExtremeUnaliving {

JumpPadSystem::JumpPadSystem()
    : System(typeid(PlayerComponent))
{

}

void JumpPadSystem::checkJumpPads() {
    for (auto& [id, gameObject]: m_gameObjects) {
         std::vector<CollisionData> collisions = m_gameWorld.lock()->getSystem<CollisionSystem>()->getEllipsoidTriangleCollisions()[gameObject->ID()];
         for (CollisionData& collision: collisions) {
             if (!m_gameWorld.lock()->hasGameObject(collision.otherID)) continue;
             if (m_gameWorld.lock()->getGameObject(collision.otherID)->hasComponentOfType(typeid(JumpPadComponent))) {
                 float jumpPower = m_gameWorld.lock()->getGameObject(collision.otherID)->getComponent<JumpPadComponent>()->getJumpPadForce();
                 glm::vec3 curVelocity = gameObject->getComponent<RigidBodyComponent>()->getVelocity();
                 gameObject->getComponent<RigidBodyComponent>()->setVelocity(curVelocity + glm::vec3(0, jumpPower, 0));
                 gameObject->getComponent<TransformComponent>()->getGlobalTransform()->translate(glm::vec3(0, 1, 0));
                 break;
             }
         }
    }

}



}
