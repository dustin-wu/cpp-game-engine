#include "charactercontrollersystem.h"
#include "Engine/Components/playercomponent.h"
#include "Engine/Components/rigidbodycomponent.h"
#include "Engine/Components/transformcomponent.h"
#include "Engine/Systems/collisionsystem.h"
#include "Engine/inputtracker.h"
#include "GLFW/glfw3.h"
#include <iostream>

CharacterControllerSystem::CharacterControllerSystem()
    : System(typeid(PlayerComponent))
    , gravity(9.8)
{}

void CharacterControllerSystem::updateCharacterTransform(double deltaTime) {
    float deltaTimef = (float)deltaTime;

    std::shared_ptr<GameObject> character = m_gameObjects[m_gameObjects.begin()->first]; // There should only be one controllable character
    std::shared_ptr<PlayerComponent> playerComponent = character->getComponent<PlayerComponent>();
    std::shared_ptr<RigidBodyComponent> rigidBodyComponent = character->getComponent<RigidBodyComponent>();
    float movementSpeed = playerComponent->getMovementSpeed();
    float headTurn = playerComponent->getHeadTurn();
    float headTilt = playerComponent->getHeadTilt();
    float jumpPower = playerComponent->getJumpPower();
    std::shared_ptr<TransformComponent> transformComponent = character->getComponent<TransformComponent>();
    std::shared_ptr<ModelTransform> transform = transformComponent->getGlobalTransform();
    oldPos = transform->getPos();
    glm::vec3 lookVec = glm::normalize(glm::vec3(glm::column(transform->getModelMatrix(), 2)));
    glm::vec3 forwardDir = glm::normalize(glm::vec3(lookVec.x, 0, lookVec.z));
    glm::vec3 sidewaysDir = glm::normalize(glm::vec3(lookVec.z, 0, -lookVec.x));

    // First person / third person
    playerComponent->setThirdPersonFactor(playerComponent->getThirdPersonFactor() - Input::tracker.getScrollDistance() / 20);
    playerComponent->setThirdPersonFactor(fmin(1, fmax(0, playerComponent->getThirdPersonFactor())));

    // WASD Movement
    playerComponent->setMovingLaterally(false);
    glm::vec3 movementVec = glm::vec3(0);
    if (Input::tracker.isKeyDown(GLFW_KEY_W)) movementVec += forwardDir;
    else if (Input::tracker.isKeyDown(GLFW_KEY_S)) movementVec -= forwardDir;
    if (Input::tracker.isKeyDown(GLFW_KEY_A)) movementVec += sidewaysDir;
    else if (Input::tracker.isKeyDown(GLFW_KEY_D)) movementVec -= sidewaysDir;
    if (glm::length(movementVec) > 0) {
        playerComponent->setMovingLaterally(true);
        movementVec = deltaTimef * movementSpeed * glm::normalize(movementVec);
    }
    transformComponent->getGlobalTransform()->translate(movementVec);

    // Turning + head movement
    glm::vec2 deltaMousPos = Input::tracker.getDeltaMousePos();
    transformComponent->getGlobalTransform()->rotate(-deltaMousPos.x * 0.005f, glm::vec3(0, 1, 0));
    playerComponent->setHeadTurn(headTurn + deltaMousPos.x * 0.005f);
    playerComponent->setHeadTilt(headTilt + deltaMousPos.y * 0.002f);


    // Jumping
    bool onGround = transformComponent->getGlobalTransform()->getPos().y <= 0;
    bool collisionBelow = m_gameWorld.lock()->getSystem<CollisionSystem>()->getCollisionBelowMap()[character->ID()];
    if (Input::tracker.isKeyDown(GLFW_KEY_SPACE) && (onGround || collisionBelow)) {
        glm::vec3 curVelocity = rigidBodyComponent->getVelocity();
        rigidBodyComponent->setVelocity(glm::vec3(curVelocity.x, jumpPower, curVelocity.z));
    }


}

void CharacterControllerSystem::fixMovementIfStationary() {
    std::shared_ptr<GameObject> character = m_gameObjects[m_gameObjects.begin()->first]; // There should only be one controllable character
    std::shared_ptr<PlayerComponent> playerComponent = character->getComponent<PlayerComponent>();
    std::shared_ptr<TransformComponent> transformComponent = character->getComponent<TransformComponent>();
    std::shared_ptr<RigidBodyComponent> rigidBodyComponent = character->getComponent<RigidBodyComponent>();
    bool onGround = oldPos.y <= 0;
    bool collisionBelow = m_gameWorld.lock()->getSystem<CollisionSystem>()->getCollisionBelowMap()[character->ID()];
    if (!playerComponent->getMovingLaterally() && (onGround || collisionBelow)) {
        transformComponent->getGlobalTransform()->setPos(oldPos);
    }
}

glm::vec3 CharacterControllerSystem::getCharacterPos() {
    std::shared_ptr<GameObject> character = m_gameObjects[m_gameObjects.begin()->first];
    return character->getComponent<TransformComponent>()->getGlobalTransform()->getPos();
}

