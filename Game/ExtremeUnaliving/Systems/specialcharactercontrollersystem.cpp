#include "specialcharactercontrollersystem.h"
#include "Engine/Components/drawableanimationcomponent.h"
#include "Engine/Components/playercomponent.h"
#include "Engine/Components/rigidbodycomponent.h"
#include "Engine/Components/transformcomponent.h"
#include "Engine/Systems/collisionsystem.h"
#include "Engine/Systems/particlesystem.h"
#include "Engine/inputtracker.h"
#include "GLFW/glfw3.h"
#include "Game/ExtremeUnaliving/Components/bulletcomponent.h"
#include "Game/ExtremeUnaliving/ShopHandler.h"
#include "Game/ExtremeUnaliving/Systems/bulletsystem.h"
#include "Game/ExtremeUnaliving/Systems/healthsystem.h"
#include "Graphics/Animation/loaders/entityanimationloader.h"
#include "Graphics/Particles/particletexture.h"
#include <unordered_set>
#include <Engine/Components/cameracomponent.h>
#include <Engine/Components/CollisionComponents/trianglecollisioncomponent.h>

namespace ExtremeUnaliving {

SpecialCharacterControllerSystem::SpecialCharacterControllerSystem()
    : isDashing(false)
    , isWallJumping(false)
    , isWallSliding(false)
    , canJump(true)
    , canDash(true)
    , maxDashDuration(0.5f)
    , dashSpeed(10.0f)
    , maxWallJumpDuration(0.5f)
    , wallJumpSpeed(3.0f)
    , maxStamina(3.0f)
    , staminaRecoveryRate(0.375f)
    , maxNumWallJumps(3)
{
    stamina = maxStamina;
    numWallJumps = maxNumWallJumps;
    playerRunning = EntityAnimationLoader::loadAnimation("Resources/Animations/playerrunning.dae");
    playerIdle = EntityAnimationLoader::loadAnimation("Resources/Animations/playeridle.dae");
    playerJumping = EntityAnimationLoader::loadAnimation("Resources/Animations/playerjumping.dae");
    ParticleTexture pt = ParticleTexture("staminaparticle", 5);
    m_particleSystem = std::make_shared<ParticleSystem>(pt, 1.f, 3.f, 1.f, 1.f, 0.1f);

}

void SpecialCharacterControllerSystem::updateCharacterTransform(double deltaTime) {
    if (ShopHandling::handler.itemBought(2)) {
        staminaRecoveryRate = 0.75f;
    }

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

    bool onGround = transformComponent->getGlobalTransform()->getPos().y <= 0;
    bool collisionBelow = m_gameWorld.lock()->getSystem<CollisionSystem>()->getCollisionBelowMap()[character->ID()];

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

    if (glm::length(movementVec) == 0) {
        character->getComponent<DrawableAnimationComponent>()->setAnimation(playerIdle);
    } else {
        character->getComponent<DrawableAnimationComponent>()->setAnimation(playerRunning);
    }

    // Turning + head movement
    glm::vec2 deltaMousPos = Input::tracker.getDeltaMousePos();
    transformComponent->getGlobalTransform()->rotate(-deltaMousPos.x * 0.005f, glm::vec3(0, 1, 0));
    playerComponent->setHeadTurn(headTurn + deltaMousPos.x * 0.005f);
//    playerComponent->setHeadTilt(headTilt + deltaMousPos.y * 0.002f);
    float newHeadTilt = headTilt + deltaMousPos.y * 0.002f;
    if (newHeadTilt > 0) {
        newHeadTilt = fmin(newHeadTilt, 1.5f);
    } else {
        newHeadTilt = fmax(newHeadTilt, -1.5f);
    }
    playerComponent->setHeadTilt(newHeadTilt);
//    std::cout << playerComponent->getHeadTilt() << std::endl;

    // Jumping
    if (Input::tracker.isKeyDown(GLFW_KEY_SPACE) && (onGround || collisionBelow) && canJump) {
        playerComponent->setMovingLaterally(true);
        canJump = false;
        glm::vec3 curVelocity = rigidBodyComponent->getVelocity();
        rigidBodyComponent->setVelocity(glm::vec3(curVelocity.x, jumpPower, curVelocity.z));
    }

    if (!canJump || rigidBodyComponent->getVelocity().y > 0) {
        character->getComponent<DrawableAnimationComponent>()->setAnimation(playerJumping);
    }

    if (!Input::tracker.isKeyDown(GLFW_KEY_SPACE)) {
        canJump = true;
    }

    float modifiedDashSpeed = dashSpeed;
    float modifiedDashDuration = maxDashDuration;
    glm::vec3 wallNormal;
    bool collisionWithWall = m_gameWorld.lock()->getSystem<CollisionSystem>()->getCollisionWithWall(character->ID(), wallNormal);
    if (collisionWithWall && !(onGround || collisionBelow)) {
        // Sliding along wall
        if (rigidBodyComponent->getVelocity().y < 0) {
//            modifiedDashSpeed *= 3.f;
            modifiedDashDuration *= 2.f;
        }
    }

    // Dashing
    if (Input::tracker.isKeyDown(GLFW_KEY_LEFT_SHIFT) && !isDashing && stamina >= 1 && canDash) {
        playerComponent->setMovingLaterally(true);
        isDashing = true;
        canDash = false;
        stamina--;
        m_particleSystem->generateParticles(transformComponent->getGlobalTransform()->getPos() + 1.f*forwardDir + glm::vec3(0,1,0), 5);

        if (glm::length(movementVec) == 0) {
            dashVector = modifiedDashSpeed*forwardDir;
        } else {
            dashVector = modifiedDashSpeed*glm::normalize(movementVec);
        }
        glm::vec3 curVelocity = rigidBodyComponent->getVelocity();
        rigidBodyComponent->setVelocity(curVelocity + dashVector);
        dashDuration = modifiedDashDuration;
    }

    if (isDashing) {
        playerComponent->setMovingLaterally(true);
        dashDuration -= deltaTimef;
        if (dashDuration <= 0) {
            isDashing = false;
            glm::vec3 curVelocity = rigidBodyComponent->getVelocity();
            rigidBodyComponent->setVelocity(curVelocity - dashVector);
        }
    }

    if (!Input::tracker.isKeyDown(GLFW_KEY_LEFT_SHIFT)) {
        canDash = true;
    }


    if (stamina < maxStamina) {
        stamina += deltaTimef * staminaRecoveryRate;
        stamina = fmin(stamina, maxStamina);
    }

    if (onGround || collisionBelow) {
        numWallJumps = maxNumWallJumps;
    }


    rigidBodyComponent->setGravity(9.8f);

    // Against a wall
    if (collisionWithWall && !(onGround || collisionBelow)) {
        // Sliding along wall
        if (rigidBodyComponent->getVelocity().y < 0) {
            rigidBodyComponent->setGravity(9.8f * 0.1f);
        }

        // Wall jump
        if (numWallJumps > 0 && Input::tracker.isKeyDown(GLFW_KEY_SPACE) && !isWallJumping && canJump) {
            isWallJumping = true;
            canJump = false;
            numWallJumps--;
            wallJumpVector = wallNormal*wallJumpSpeed - movementVec;
            glm::vec3 curVelocity = rigidBodyComponent->getVelocity();
            rigidBodyComponent->setVelocity(curVelocity + wallJumpVector);
            curVelocity = rigidBodyComponent->getVelocity();
            rigidBodyComponent->setVelocity(glm::vec3(curVelocity.x, jumpPower, curVelocity.z));
            wallJumpDuration = maxWallJumpDuration;
        }
    }

//    std::cout << rigidBodyComponent->getGravity() << std::endl;

    if (isWallJumping) {
        glm::vec3 curVelocity = rigidBodyComponent->getVelocity();
        transformComponent->getGlobalTransform()->translate(-movementVec);

        wallJumpDuration -= deltaTime;
        if (wallJumpDuration <= 0) {
            isWallJumping = false;

            rigidBodyComponent->setVelocity(curVelocity - wallJumpVector);
        }
    }

//     std::cout << "glm::vec3(" << oldPos.x << ", " << oldPos.y << ", " << oldPos.z << ")" << std::endl;

    checkCameraColliding(deltaTime, forwardDir);

}

void SpecialCharacterControllerSystem::checkHitProjectile() {
    std::unordered_set<GameObjectID> bulletsUsed;
    std::vector<CollisionInfo> collisions = m_gameWorld.lock()->getSystem<CollisionSystem>()->getCollisions()[m_gameObjects.begin()->first];
    for (CollisionInfo collision: collisions) {
        if (!m_gameWorld.lock()->hasGameObject(collision.otherID)) continue;

        if (m_gameWorld.lock()->getGameObject(collision.otherID)->hasComponentOfType(typeid(BulletComponent))) {
            bulletsUsed.insert(collision.otherID);
            m_gameWorld.lock()->getSystem<BulletSystem>()->addToRemove(collision.otherID);
            m_gameWorld.lock()->getSystem<HealthSystem>()->takeDamage(10);
        }
    }
}

void SpecialCharacterControllerSystem::checkCameraColliding(double deltaTime, glm::vec3 forwards) {
    std::shared_ptr<GameObject> character = m_gameObjects[m_gameObjects.begin()->first]; // There should only be one controllable character

    // Camera Collider follows camera
    m_gameWorld.lock()->getGameObject(m_wallCheckID)->getComponent<TransformComponent>()->getGlobalTransform()->setPos(getCharacterPos() - 0.8f * forwards + glm::vec3(0, 1, 0));

    std::vector<CollisionData> collisions = m_gameWorld.lock()->getSystem<CollisionSystem>()->getEllipsoidTriangleCollisions()[m_wallCheckID];
    if (collisions.size() > 0) {
        // Increase near to avoid staring at wall
        character->getComponent<CameraComponent>()->getCamera()->setNear(1.5f);
    } else {
        character->getComponent<CameraComponent>()->getCamera()->setNear(0.1f);
    }

}



}
