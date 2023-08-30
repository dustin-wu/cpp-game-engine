#include "gunsystem.h"
#include "Engine/Components/playercomponent.h"
#include "Engine/Components/transformcomponent.h"
#include "Engine/Systems/collisionsystem.h"
#include "Engine/inputtracker.h"
#include "GLFW/glfw3.h"
#include "Game/PrairieKing3D/Components/guncomponent.h"
#include "Game/PrairieKing3D/Systems/initializersystem.h"

namespace PrairieKing3D {

GunSystem::GunSystem()
    : System(typeid(GunComponent))
{
    defaultGunMat = glm::mat4(1);
    defaultGunMat = glm::translate(defaultGunMat, glm::vec3(0, 0.73, 0.5));
    reset();
}

void GunSystem::reset() {
    gunMat = defaultGunMat;
    ammoCount = 6;
    maxAmmoCount = 6;
    reloadTime = 3.f;
    maxReloadTime = 3.f;
    coolDown = 0;
    maxCoolDown = 0.5f;
    gunReload = false;
    manualReload = false;
}

void GunSystem::updateGunTransform(float deltaTimef) {
    std::shared_ptr<GameObject> gunObject = m_gameObjects[m_gameObjects.begin()->first];
    GameObjectID playerID = gunObject->getComponent<GunComponent>()->getPlayerID();
    std::shared_ptr<GameObject> playerObject = m_gameWorld.lock()->getGameObject(playerID);
    glm::mat4 playerMat = playerObject->getComponent<TransformComponent>()->getGlobalTransform()->getModelMatrix();
    glm::mat4 tiltedGunMat;

    if (gunReload) {
        gunMat = glm::rotate(gunMat, (float)((2 * M_PI / maxReloadTime) * deltaTimef), glm::vec3(1, 0, 0));
        tiltedGunMat = gunMat;
    } else {
        tiltedGunMat = glm::rotate(defaultGunMat, playerObject->getComponent<PlayerComponent>()->getHeadTilt(), glm::vec3(1, 0, 0));
    }
    glm::mat4 combinedMat = playerMat * tiltedGunMat;
    gunObject->getComponent<TransformComponent>()->getGlobalTransform()->setModelMatrix(combinedMat);
}

void GunSystem::fireBulletOnMouse(float deltaTimef) {
    coolDown -= deltaTimef;

    if (Input::tracker.isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && coolDown <= 0 && ammoCount >= 1) {
        std::shared_ptr<GameObject> gunObject = m_gameObjects[m_gameObjects.begin()->first];
        std::shared_ptr<ModelTransform> bulletTransform = std::make_shared<ModelTransform>(*gunObject->getComponent<TransformComponent>()->getGlobalTransform());
        m_gameWorld.lock()->getSystem<InitializerSystem>()->addBullet(bulletTransform);
        coolDown = maxCoolDown;
        ammoCount--;
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
