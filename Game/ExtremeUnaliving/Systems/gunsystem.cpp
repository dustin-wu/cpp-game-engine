#include "gunsystem.h"
#include "Engine/Components/cameracomponent.h"
#include "Engine/Components/drawableanimationcomponent.h"
#include "Engine/Components/drawablecomponent.h"
#include "Engine/Components/playercomponent.h"
#include "Engine/Components/transformcomponent.h"
#include "Engine/Systems/camerasystem.h"
#include "Engine/Systems/collisionsystem.h"
#include "Engine/inputtracker.h"
#include "Game/ExtremeUnaliving/Components/guncomponent.h"
#include "Game/ExtremeUnaliving/ShopHandler.h"
#include "Game/ExtremeUnaliving/Systems/enemysystem.h"
#include "Game/ExtremeUnaliving/Systems/gameinitializersystem.h"
#include "Graphics/global.h"
#include "GLFW/glfw3.h"

namespace ExtremeUnaliving {

GunSystem::GunSystem()
    : System(typeid(GunComponent))
{
    defaultGunMat = glm::mat4(1);
    defaultGunMat = glm::translate(defaultGunMat, glm::vec3(0, 0.73, 0.5));
    ParticleTexture pt = ParticleTexture("bulletsparks", 1);
    m_particleSystem = std::make_shared<ParticleSystem>(pt, 1.f, 3.f, 1.f, 1.f, 0.05f);

    reset();
}

std::shared_ptr<ModelTransform> GunSystem::getGunTransform() {
    std::shared_ptr<GameObject> gunObject = m_gameObjects[m_gameObjects.begin()->first];
    return gunObject->getComponent<TransformComponent>()->getGlobalTransform();
}

void GunSystem::reset() {
    gunMat = defaultGunMat;
    ammoCount = 6;
    maxAmmoCount = 6;
    reloadTime = 1.5f;
    maxReloadTime = 1.5f;
    coolDown = 0;
    maxCoolDown = 0.3f;
    gunReload = false;
    manualReload = false;
    if (sprayPurchased) {
        std::shared_ptr<GameObject> gunObject = m_gameObjects[m_gameObjects.begin()->first];
        gunObject->getComponent<DrawableComponent>()->setShape("gunMesh");
        gunObject->getComponent<DrawableComponent>()->getLocalTransform()->setScale(0.07f);
        gunObject->getComponent<DrawableComponent>()->getLocalTransform()->rotate(M_PI, glm::vec3(0, 1, 0));
    }
    sprayPurchased = false;
    gunRange = 100.f;

}

void GunSystem::spraynpray() {
    std::shared_ptr<GameObject> gunObject = m_gameObjects[m_gameObjects.begin()->first];
    gunObject->getComponent<DrawableComponent>()->setShape("sprayMesh");
    gunObject->getComponent<DrawableComponent>()->getLocalTransform()->setScale(0.4f);
    gunObject->getComponent<DrawableComponent>()->getLocalTransform()->rotate(M_PI, glm::vec3(0, 1, 0));
    ammoCount = 100;
    maxAmmoCount = 100;
    reloadTime = 10.f;
    maxReloadTime = 10.f;
    coolDown = 0;
    maxCoolDown = 0.1f;
}

void GunSystem::updateGunTransform(float deltaTimef) {
    if (ShopHandling::handler.itemBought(0) && !sprayPurchased) {
        spraynpray();
        sprayPurchased = true;
    }

    std::shared_ptr<GameObject> gunObject = m_gameObjects[m_gameObjects.begin()->first];
    GameObjectID playerID = m_gameWorld.lock()->getPlayerID();
    std::shared_ptr<GameObject> playerObject = m_gameWorld.lock()->getGameObject(playerID);
    glm::mat4 playerMat = playerObject->getComponent<TransformComponent>()->getGlobalTransform()->getModelMatrix();
    glm::mat4 playerRot = playerObject->getComponent<TransformComponent>()->getGlobalTransform()->getRotation();

    glm::mat4 handModel = playerObject->getComponent<DrawableAnimationComponent>()->getEntity()->getJointTransforms()[9];
    glm::mat4 handLocal = playerObject->getComponent<DrawableAnimationComponent>()->getLocalTransform()->getModelMatrix();

    glm::mat4 tiltedGunMat;
    if (gunReload) {
        if (reloadTime > maxReloadTime/2.f) {
            gunMat = glm::rotate(gunMat, (float)((2 * M_PI / maxReloadTime) * deltaTimef), glm::vec3(1, 0, 0));
            tiltedGunMat = gunMat;
        } else {
            gunMat = glm::rotate(gunMat, -(float)((2 * M_PI / maxReloadTime) * deltaTimef), glm::vec3(1, 0, 0));
            tiltedGunMat = gunMat;
        }
    } else {
        tiltedGunMat = glm::rotate(defaultGunMat, playerObject->getComponent<PlayerComponent>()->getHeadTilt(), glm::vec3(1, 0, 0));
    }


    glm::mat4 handWorldModel = playerMat * handLocal * handModel;
    glm::vec3 handWorldTranslation = glm::vec3(handWorldModel[3]);


    gunObject->getComponent<TransformComponent>()->getGlobalTransform()->setRotation(playerRot * tiltedGunMat);
    gunObject->getComponent<TransformComponent>()->getGlobalTransform()->setPos(handWorldTranslation + glm::vec3(playerRot * glm::vec4(-0.52, -1, -0.67, 1)));


}

void GunSystem::drawGunfire() {
    if (gunfireDuration > 0) {
        Global::graphics.bindShader("beam");
        Global::graphics.setGlobalData(glm::vec3(0.5f));
        Global::graphics.setCameraData(m_gameWorld.lock()->getSystem<CameraSystem>()->getActiveCamera());
        Global::graphics.drawBeam(gunfireStart, gunfireEnd, 0.05f * gunfireDuration / 0.15f, "beamgradient");
    }
}

void GunSystem::drawGunParticles() {
    m_particleSystem->generateParticles(gunfireEnd, 7);
}

void GunSystem::fireBulletOnMouse(float deltaTimef) {
    coolDown -= deltaTimef;
    GameObjectID playerID = m_gameWorld.lock()->getPlayerID();
    std::shared_ptr<GameObject> playerObject = m_gameWorld.lock()->getGameObject(playerID);

    if (Input::tracker.isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && coolDown <= 0 && ammoCount >= 1) {
        glm::vec3 rayStart = playerObject->getComponent<CameraComponent>()->getCamera()->getPos() + 2.5f * playerObject->getComponent<CameraComponent>()->getCamera()->getLook();
        glm::vec3 rayDir = playerObject->getComponent<CameraComponent>()->getCamera()->getLook();
        float hitEnvT = m_gameWorld.lock()->getSystem<CollisionSystem>()->rayTraceTriangles(rayStart, rayDir);
        GameObjectID closestHit;
        float bestT = std::numeric_limits<float>::max();
        for (GameObjectID enemyID: m_gameWorld.lock()->getSystem<EnemySystem>()->getEnemies()) {
            float candT = m_gameWorld.lock()->getSystem<CollisionSystem>()->checkRayHit(enemyID, rayStart, rayDir);
            if (candT < bestT) {
                bestT = candT;
                closestHit = enemyID;
            }
        }

        float t = fmin(hitEnvT, bestT);
        glm::vec3 rayEnd = rayStart + t * rayDir;
        gunfireEnd = rayEnd;
        gunfireDuration = 0.15f;

        if (bestT < hitEnvT && bestT <= gunRange) {
            m_gameWorld.lock()->getSystem<EnemySystem>()->takeDamage(closestHit, gunfireEnd);
        }

        std::shared_ptr<GameObject> gunObject = m_gameObjects[m_gameObjects.begin()->first];

        std::shared_ptr<ModelTransform> bulletTransform = std::make_shared<ModelTransform>(*gunObject->getComponent<TransformComponent>()->getGlobalTransform());
        bulletTransform->setModelMatrix(bulletTransform->getModelMatrix() * glm::translate(glm::mat4(1), glm::vec3(-0.145, 0.14, 0.9)));
        gunfireStart = bulletTransform->getPos();

        coolDown = maxCoolDown;
        ammoCount--;
        drawGunParticles();
    } else {
        gunfireDuration -= deltaTimef;
    }

    if (Input::tracker.isKeyDown(GLFW_KEY_R) && !gunReload && !manualReload && ammoCount < maxAmmoCount) {
        manualReload = true;
    }

    if (ammoCount == 0 || manualReload) {
        gunReload = true;
        if (reloadTime <= 0) {
            ammoCount = maxAmmoCount;
            reloadTime = maxReloadTime;
            gunReload = false;
            manualReload = false;
            gunMat = defaultGunMat;
        } else {
            reloadTime -= deltaTimef;
        }
    }

}

}
