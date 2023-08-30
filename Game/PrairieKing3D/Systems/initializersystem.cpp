#include "initializersystem.h"
#include "Engine/Components/nocomponent.h"
#include "Engine/Components/CollisionComponents/cylindercollisioncomponent.h"
#include "Engine/Components/cameracomponent.h"
#include "Engine/Components/drawablecomponent.h"
#include "Engine/Components/playercomponent.h"
#include "Engine/Components/rigidbodycomponent.h"
#include "Engine/Components/transformcomponent.h"
#include "Engine/Systems/camerasystem.h"
#include "Engine/Systems/charactercontrollersystem.h"
#include "Engine/Systems/collisionsystem.h"
#include "Engine/Systems/drawsystem.h"
#include "Engine/Systems/physicssystem.h"
#include "Game/PrairieKing3D/Components/bulletcomponent.h"
#include "Game/PrairieKing3D/Components/enemycomponent.h"
#include "Game/PrairieKing3D/Components/guncomponent.h"
#include "Game/PrairieKing3D/Systems/bulletsystem.h"
#include "Game/PrairieKing3D/Systems/enemysystem.h"
#include "Game/PrairieKing3D/Systems/gunsystem.h"
#include "Game/PrairieKing3D/Systems/healthsystem.h"
#include "Graphics/global.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace PrairieKing3D {

InitializerSystem::InitializerSystem()
    : System(typeid(NoComponent))
{

}

void InitializerSystem::initalizeGame() {
    std::shared_ptr<GameWorld> gameWorld = m_gameWorld.lock();

    Global::graphics.addMaterial("grassTexture", "Resources/Images/grass.png");
    Global::graphics.addMaterial("obamaTexture", "Resources/Images/obama2.png");
    Global::graphics.addMaterial("trumpTexture", "Resources/Images/trump.jpg");
    Global::graphics.addMaterial("sandTexture", "Resources/Images/sand2.jpeg");
    Global::graphics.addMaterial("wallTexture", "Resources/Images/cactus.jpeg");
    Global::graphics.addMaterial("pantsTexture", "Resources/Images/pants.png");
    Global::graphics.addMaterial("shirtTexture", "Resources/Images/shirt.jpg");
    Global::graphics.addMaterial("hatTexture", "Resources/Images/hat.png");
    Global::graphics.addMaterial("gunTexture", "Resources/Images/gun.jpg");
    Global::graphics.addMaterial("bulletTexture", "Resources/Images/bullet.jpg");
    Global::graphics.addMaterial("zombieTexture", "Resources/Images/zombie.jpg");

    Global::graphics.addShape("testMesh", "Resources/Meshes/ceiling.obj");

    std::vector<std::shared_ptr<Light>> lights;
    std::shared_ptr<Light> light = std::make_shared<Light>(LightType::DIRECTIONAL, glm::vec3(1, -1, 1));

    lights.push_back(light);
    Global::graphics.setLights(lights);

    gameWorld->addSystem<DrawSystem>();
    gameWorld->addSystem<CharacterControllerSystem>();
    gameWorld->addSystem<CameraSystem>();
    gameWorld->addSystem<PhysicsSystem>();
    gameWorld->addSystem<CollisionSystem>();
    gameWorld->addSystem<GunSystem>();
    gameWorld->addSystem<BulletSystem>();
    gameWorld->addSystem<EnemySystem>();
    gameWorld->addSystem<HealthSystem>();

    initializePlayer();
}

void InitializerSystem::loadGame(std::string fileName) {
    initializeMap(fileName);
    m_gameWorld.lock()->reset();
}

void InitializerSystem::addFloor(std::shared_ptr<GameObject> floorObject, glm::vec3 floorPos) {
    std::shared_ptr<DrawableComponent> floorDrawable = std::make_shared<DrawableComponent>();
    floorDrawable->setShape("quad");
    floorDrawable->setMaterial("sandTexture");
    floorDrawable->getLocalTransform()->translate(floorPos);
    floorObject->addComponent<DrawableComponent>(floorDrawable);
    mapObjects.push_back(floorObject->ID());
}

void InitializerSystem::addWall(glm::vec3 wallPos) {
    std::shared_ptr<GameObject> wallObject = std::make_shared<GameObject>();
    std::shared_ptr<TransformComponent> wallTransform = std::make_shared<TransformComponent>();
    std::shared_ptr<ModelTransform> wallTransformDefault = std::make_shared<ModelTransform>();
    wallTransformDefault->setPos(wallPos);
    wallTransform->setDefaultTransform(wallTransformDefault);
    wallObject->addComponent<TransformComponent>(wallTransform);
    std::shared_ptr<DrawableComponent> wallDrawable = std::make_shared<DrawableComponent>();
    wallDrawable->setShape("cylinder");
    wallDrawable->setMaterial("wallTexture");
    wallDrawable->getLocalTransform()->setScale(glm::vec3(1, 2, 1));
    wallDrawable->getLocalTransform()->setPos(glm::vec3(0, 1, 0));
    wallDrawable->getLocalTransform()->setRotation(-M_PI / 2, glm::vec3(0, 1, 0));
    wallObject->addComponent<DrawableComponent>(wallDrawable);
    std::shared_ptr<RigidBodyComponent> wallRigidBody = std::make_shared<RigidBodyComponent>();
    wallRigidBody->setStationary(true);
    wallObject->addComponent<RigidBodyComponent>(wallRigidBody);
    std::shared_ptr<CylinderCollisionComponent> wallCollider = std::make_shared<CylinderCollisionComponent>();
    wallCollider->setHeight(1);
    wallCollider->setLocalPos(glm::vec3(0, 0.5, 0));
    wallObject->addComponent<CylinderCollisionComponent>(wallCollider);
    m_gameWorld.lock()->addGameObject(wallObject);
    mapObjects.push_back(wallObject->ID());
}

void InitializerSystem::initializeMap(std::string fileName) {
    for (GameObjectID mapObject: mapObjects) {
        m_gameWorld.lock()->removeGameObject(mapObject);
    }

    glm::vec3 curOffset = glm::vec3(0, 0, 0);
    std::shared_ptr<GameObject> floorObject = std::make_shared<GameObject>();
    std::shared_ptr<TransformComponent> floorTransform = std::make_shared<TransformComponent>();
    floorObject->addComponent<TransformComponent>(floorTransform);
    std::ifstream file(fileName);
    std::vector<glm::vec3> spawnAreas;
    std::vector<float> spawnAngles;
    std::queue<int> waveCounts;
    std::queue<float> waveCooldowns;

    if (file.is_open()) {
        std::cout << "Read file: " << fileName << std::endl;

        std::string firstLine;
        std::getline(file, firstLine);
        std::istringstream ss1(firstLine);
        for (std::string waveToken; ss1 >> waveToken; ) {
            std::istringstream ss2(waveToken);
            std::string numToken;
            std::getline(ss2, numToken, ',');
            int numEnemies = std::stoi(numToken);
            std::getline(ss2, numToken);
            float waveCooldown = std::stof(numToken);
            waveCounts.push(numEnemies);
            waveCooldowns.push(waveCooldown);
            //m_gameWorld.lock()->getSystem<EnemySystem>()->addWave(numEnemies, waveCooldown);
        }
        m_gameWorld.lock()->getSystem<EnemySystem>()->setWaves(waveCounts, waveCooldowns);

        std::string line;
        while (std::getline(file, line)) {
            for (char c: line) {
                if (c == 'W') {
                    addWall(curOffset);
                }
                else if (c == 'D') {
                    spawnAreas.push_back(curOffset + glm::vec3(0, 0.5, 0));
                    spawnAngles.push_back(M_PI / 2);
                }
                else if (c == 'L') {
                    spawnAreas.push_back(curOffset + glm::vec3(0, 0.5, 0));
                    spawnAngles.push_back(0);
                }
                else if (c == 'R') {
                    spawnAreas.push_back(curOffset + glm::vec3(0, 0.5, 0));
                    spawnAngles.push_back(M_PI);
                }
                else if (c == 'U') {
                    spawnAreas.push_back(curOffset + glm::vec3(0, 0.5, 0));
                    spawnAngles.push_back(- M_PI / 2);
                }
                else if (c == 'P') {
                    std::shared_ptr<ModelTransform> playerTransformDefault = std::make_shared<ModelTransform>();
                    playerTransformDefault->setPos(curOffset);
                    m_gameWorld.lock()->getGameObject(playerID)->getComponent<TransformComponent>()->setDefaultTransform(playerTransformDefault);
//                    m_gameWorld.lock()->getGameObject(playerID)->getComponent<TransformComponent>()->reset();
                }
                addFloor(floorObject, curOffset);
                curOffset += glm::vec3(1, 0, 0);
            }
            curOffset = glm::vec3(0, 0, curOffset.z);
            curOffset += glm::vec3(0, 0, 1);
        }
        // Close the file
        file.close();
    } else {
        std::cout << "Error: could not open file" << std::endl;
    }
    m_gameWorld.lock()->addGameObject(floorObject);
    m_gameWorld.lock()->getSystem<EnemySystem>()->setSpawnAreas(spawnAreas);
    m_gameWorld.lock()->getSystem<EnemySystem>()->setSpawnAngles(spawnAngles);

}

void InitializerSystem::addBullet(std::shared_ptr<ModelTransform> bulletModel) {
    std::shared_ptr<GameObject> bulletObject = std::make_shared<GameObject>();
    std::shared_ptr<BulletComponent> bulletConfig = std::make_shared<BulletComponent>();
    bulletObject->addComponent<BulletComponent>(bulletConfig);
    std::shared_ptr<TransformComponent> bulletTransform = std::make_shared<TransformComponent>();
    glm::vec3 bulletForward = glm::vec3(bulletModel->getRotation() * glm::vec4(0, 0, 1, 0));
    bulletTransform->setDefaultTransform(bulletModel);
    bulletTransform->getGlobalTransform()->translate(0.2f * bulletForward);
    bulletObject->addComponent<TransformComponent>(bulletTransform);
    std::shared_ptr<RigidBodyComponent> bulletRigidBody = std::make_shared<RigidBodyComponent>();
    bulletRigidBody->setAffectedByGravity(false);
    bulletRigidBody->setVelocity(bulletConfig->getSpeed() * bulletForward);
    bulletObject->addComponent<RigidBodyComponent>(bulletRigidBody);
    std::shared_ptr<CylinderCollisionComponent> bulletCollider = std::make_shared<CylinderCollisionComponent>();
    bulletCollider->setRadius(0.1);
    bulletCollider->setHeight(0.1);
    bulletObject->addComponent<CylinderCollisionComponent>(bulletCollider);
    std::shared_ptr<DrawableComponent> bulletDraw = std::make_shared<DrawableComponent>();
    bulletDraw->setShape("cylinder");
    bulletDraw->setMaterial("bulletTexture");
    bulletDraw->getLocalTransform()->setScale(glm::vec3(0.1, 0.15, 0.1));
    bulletDraw->getLocalTransform()->setRotation(M_PI / 2, glm::vec3(1, 0, 0));
    bulletObject->addComponent<DrawableComponent>(bulletDraw);
    m_gameWorld.lock()->addGameObject(bulletObject);
}

void InitializerSystem::initializePlayer() {
    std::shared_ptr<GameObject> playerObject = std::make_shared<GameObject>();
    std::shared_ptr<TransformComponent> playerTransform = std::make_shared<TransformComponent>();
    std::shared_ptr<ModelTransform> playerTransformDefault = std::make_shared<ModelTransform>();
    playerTransform->setDefaultTransform(playerTransformDefault);
    playerObject->addComponent<TransformComponent>(playerTransform);
    std::shared_ptr<CameraComponent> playerCamera = std::make_shared<CameraComponent>();
    playerObject->addComponent<CameraComponent>(playerCamera);
    std::shared_ptr<PlayerComponent> playerConfig = std::make_shared<PlayerComponent>();
    playerConfig->setEyeHeight(1);
    playerObject->addComponent<PlayerComponent>(playerConfig);
    std::shared_ptr<RigidBodyComponent> playerRigidBody = std::make_shared<RigidBodyComponent>();
    playerObject->addComponent<RigidBodyComponent>(playerRigidBody);
    std::shared_ptr<CylinderCollisionComponent> playerCollider = std::make_shared<CylinderCollisionComponent>();
    playerObject->addComponent<CylinderCollisionComponent>(playerCollider);

    std::shared_ptr<DrawableComponent> playerHat = std::make_shared<DrawableComponent>();
    playerHat->setShape("cylinder");
    playerHat->setMaterial("hatTexture");
    playerHat->getLocalTransform()->setScale(glm::vec3(0.35, 0.1, 0.35));
    playerHat->getLocalTransform()->setPos(glm::vec3(0, 1.15, 0));
    playerHat->getLocalTransform()->setRotation(M_PI / 2, glm::vec3(0, 1, 0));
    playerObject->addComponent<DrawableComponent>(playerHat);

    std::shared_ptr<DrawableComponent> playerBrim = std::make_shared<DrawableComponent>();
    playerBrim->setShape("cylinder");
    playerBrim->setMaterial("hatTexture");
    playerBrim->getLocalTransform()->setScale(glm::vec3(0.5, 0.05, 0.5));
    playerBrim->getLocalTransform()->setPos(glm::vec3(0, 1.1, 0));
    playerBrim->getLocalTransform()->setRotation(M_PI / 2, glm::vec3(0, 1, 0));
    playerObject->addComponent<DrawableComponent>(playerBrim);

    std::shared_ptr<DrawableComponent> playerHead = std::make_shared<DrawableComponent>();
    playerHead->setShape("sphere");
    playerHead->setMaterial("obamaTexture");
    playerHead->getLocalTransform()->setScale(glm::vec3(0.4, 0.4, 0.4));
    playerHead->getLocalTransform()->setPos(glm::vec3(0, 1, 0));
    playerHead->getLocalTransform()->setRotation(M_PI / 2, glm::vec3(0, 1, 0));
    playerObject->addComponent<DrawableComponent>(playerHead);

    std::shared_ptr<DrawableComponent> playerBody = std::make_shared<DrawableComponent>();
    playerBody->setShape("cylinder");
    playerBody->setMaterial("shirtTexture");
    playerBody->getLocalTransform()->setScale(glm::vec3(0.5, 0.25, 0.5));
    playerBody->getLocalTransform()->setPos(glm::vec3(0, 0.7, 0));
    playerBody->getLocalTransform()->setRotation(M_PI / 2, glm::vec3(0, 1, 0));
    playerObject->addComponent<DrawableComponent>(playerBody);

    std::shared_ptr<DrawableComponent> playerWaist = std::make_shared<DrawableComponent>();
    playerWaist->setShape("cylinder");
    playerWaist->setMaterial("pantsTexture");
    playerWaist->getLocalTransform()->setScale(glm::vec3(0.4, 0.25, 0.4));
    playerWaist->getLocalTransform()->setPos(glm::vec3(0, 0.45, 0));
    playerWaist->getLocalTransform()->setRotation(M_PI / 2, glm::vec3(0, 1, 0));
    playerObject->addComponent<DrawableComponent>(playerWaist);

    std::shared_ptr<DrawableComponent> playerRightLeg = std::make_shared<DrawableComponent>();
    playerRightLeg->setShape("cylinder");
    playerRightLeg->setMaterial("pantsTexture");
    playerRightLeg->getLocalTransform()->setScale(glm::vec3(0.1, 0.4, 0.1));
    playerRightLeg->getLocalTransform()->setPos(glm::vec3(-0.1, 0.2, 0));
    playerRightLeg->getLocalTransform()->setRotation(M_PI / 2, glm::vec3(0, 1, 0));
    playerObject->addComponent<DrawableComponent>(playerRightLeg);

    std::shared_ptr<DrawableComponent> playerLeftLeg = std::make_shared<DrawableComponent>();
    playerLeftLeg->setShape("cylinder");
    playerLeftLeg->setMaterial("pantsTexture");
    playerLeftLeg->getLocalTransform()->setScale(glm::vec3(0.1, 0.4, 0.1));
    playerLeftLeg->getLocalTransform()->setPos(glm::vec3(0.1, 0.2, 0));
    playerLeftLeg->getLocalTransform()->setRotation(M_PI / 2, glm::vec3(0, 1, 0));
    playerObject->addComponent<DrawableComponent>(playerLeftLeg);

    std::shared_ptr<DrawableComponent> playerRightArm = std::make_shared<DrawableComponent>();
    playerRightArm->setShape("cylinder");
    playerRightArm->setMaterial("shirtTexture");
    playerRightArm->getLocalTransform()->setScale(glm::vec3(0.1, 0.5, 0.1));
    playerRightArm->getLocalTransform()->setPos(glm::vec3(-0.15, 0.7, 0.3));
    playerRightArm->getLocalTransform()->setRotation(M_PI / 2, glm::vec3(1, 0, 0));
    playerRightArm->getLocalTransform()->rotate(-M_PI / 8, glm::vec3(0, 0, 1));
    playerObject->addComponent<DrawableComponent>(playerRightArm);

    std::shared_ptr<DrawableComponent> playerLeftArm = std::make_shared<DrawableComponent>();
    playerLeftArm->setShape("cylinder");
    playerLeftArm->setMaterial("shirtTexture");
    playerLeftArm->getLocalTransform()->setScale(glm::vec3(0.1, 0.5, 0.1));
    playerLeftArm->getLocalTransform()->setPos(glm::vec3(0.3, 0.5, 0));
    playerLeftArm->getLocalTransform()->setRotation(M_PI / 8, glm::vec3(0, 0, 1));
    playerObject->addComponent<DrawableComponent>(playerLeftArm);

    m_gameWorld.lock()->addGameObject(playerObject);

    playerID = playerObject->ID();
    m_gameWorld.lock()->setPlayerID(playerObject->ID());

    std::shared_ptr<GameObject> gunObject = std::make_shared<GameObject>();

    std::shared_ptr<GunComponent> gunConfig = std::make_shared<GunComponent>(playerObject->ID());
    gunObject->addComponent<GunComponent>(gunConfig);

    std::shared_ptr<TransformComponent> gunTransform = std::make_shared<TransformComponent>();
    gunObject->addComponent<TransformComponent>(gunTransform);

    std::shared_ptr<DrawableComponent> gunBarrel = std::make_shared<DrawableComponent>();
    gunBarrel->setShape("cylinder");
    gunBarrel->setMaterial("gunTexture");
    gunBarrel->getLocalTransform()->setScale(glm::vec3(0.05, 0.2, 0.05));
    gunBarrel->getLocalTransform()->setPos(glm::vec3(0, 0.05, 0.1));
    gunBarrel->getLocalTransform()->setRotation(M_PI / 2, glm::vec3(1, 0, 0));
    gunObject->addComponent<DrawableComponent>(gunBarrel);

    std::shared_ptr<DrawableComponent> gunHandle = std::make_shared<DrawableComponent>();
    gunHandle->setShape("cube");
    gunHandle->setMaterial("gunTexture");
    gunHandle->getLocalTransform()->setScale(glm::vec3(0.05, 0.1, 0.05));
    gunHandle->getLocalTransform()->setPos(glm::vec3(0, 0, 0));
    gunHandle->getLocalTransform()->setRotation(M_PI / 8, glm::vec3(1, 0, 0));
    gunObject->addComponent<DrawableComponent>(gunHandle);

    std::shared_ptr<CameraComponent> gunScope = std::make_shared<CameraComponent>();
    gunObject->addComponent<CameraComponent>(gunScope);

    m_gameWorld.lock()->addGameObject(gunObject);
}

void InitializerSystem::addEnemy(glm::vec3 enemyPos, float enemyRotation) {
    std::shared_ptr<GameObject> enemyObject = std::make_shared<GameObject>();
    std::shared_ptr<EnemyComponent> enemyConfig = std::make_shared<EnemyComponent>(playerID);
    enemyObject->addComponent<EnemyComponent>(enemyConfig);
    std::shared_ptr<TransformComponent> enemyTransform = std::make_shared<TransformComponent>();
    std::shared_ptr<ModelTransform> enemyTransformDefault = std::make_shared<ModelTransform>();
    enemyTransformDefault->setPos(enemyPos);
    enemyTransformDefault->setRotation(enemyRotation, glm::vec3(0, 1, 0));
    enemyTransform->setDefaultTransform(enemyTransformDefault);
    enemyObject->addComponent<TransformComponent>(enemyTransform);
    std::shared_ptr<RigidBodyComponent> enemyRigidBody = std::make_shared<RigidBodyComponent>();
    enemyObject->addComponent<RigidBodyComponent>(enemyRigidBody);
    std::shared_ptr<CylinderCollisionComponent> enemyCollider = std::make_shared<CylinderCollisionComponent>();
    enemyCollider->setHeight(2);
    enemyCollider->setRadius(0.4);
    enemyCollider->setLocalPos(glm::vec3(0, 0.5, 0));
    enemyObject->addComponent<CylinderCollisionComponent>(enemyCollider);

    std::shared_ptr<DrawableComponent> enemyHead = std::make_shared<DrawableComponent>();
    enemyHead->setShape("sphere");
    enemyHead->setMaterial("trumpTexture");
    enemyHead->getLocalTransform()->setScale(glm::vec3(0.4, 0.4, 0.4));
    enemyHead->getLocalTransform()->setPos(glm::vec3(0, 1, 0));
    enemyHead->getLocalTransform()->setRotation(M_PI / 2, glm::vec3(0, 1, 0));
    enemyObject->addComponent<DrawableComponent>(enemyHead);

    std::shared_ptr<DrawableComponent> enemyBody = std::make_shared<DrawableComponent>();
    enemyBody->setShape("cylinder");
    enemyBody->setMaterial("zombieTexture");
    enemyBody->getLocalTransform()->setScale(glm::vec3(0.5, 0.25, 0.5));
    enemyBody->getLocalTransform()->setPos(glm::vec3(0, 0.7, 0));
    enemyBody->getLocalTransform()->setRotation(M_PI / 2, glm::vec3(0, 1, 0));
    enemyObject->addComponent<DrawableComponent>(enemyBody);

    std::shared_ptr<DrawableComponent> enemyWaist = std::make_shared<DrawableComponent>();
    enemyWaist->setShape("cylinder");
    enemyWaist->setMaterial("zombieTexture");
    enemyWaist->getLocalTransform()->setScale(glm::vec3(0.4, 0.25, 0.4));
    enemyWaist->getLocalTransform()->setPos(glm::vec3(0, 0.45, 0));
    enemyWaist->getLocalTransform()->setRotation(M_PI / 2, glm::vec3(0, 1, 0));
    enemyObject->addComponent<DrawableComponent>(enemyWaist);

    std::shared_ptr<DrawableComponent> enemyRightLeg = std::make_shared<DrawableComponent>();
    enemyRightLeg->setShape("cylinder");
    enemyRightLeg->setMaterial("zombieTexture");
    enemyRightLeg->getLocalTransform()->setScale(glm::vec3(0.1, 0.4, 0.1));
    enemyRightLeg->getLocalTransform()->setPos(glm::vec3(-0.1, 0.2, 0));
    enemyRightLeg->getLocalTransform()->setRotation(M_PI / 2, glm::vec3(0, 1, 0));
    enemyObject->addComponent<DrawableComponent>(enemyRightLeg);

    std::shared_ptr<DrawableComponent> enemyLeftLeg = std::make_shared<DrawableComponent>();
    enemyLeftLeg->setShape("cylinder");
    enemyLeftLeg->setMaterial("zombieTexture");
    enemyLeftLeg->getLocalTransform()->setScale(glm::vec3(0.1, 0.4, 0.1));
    enemyLeftLeg->getLocalTransform()->setPos(glm::vec3(0.1, 0.2, 0));
    enemyLeftLeg->getLocalTransform()->setRotation(M_PI / 2, glm::vec3(0, 1, 0));
    enemyObject->addComponent<DrawableComponent>(enemyLeftLeg);

    std::shared_ptr<DrawableComponent> enemyRightArm = std::make_shared<DrawableComponent>();
    enemyRightArm->setShape("cylinder");
    enemyRightArm->setMaterial("zombieTexture");
    enemyRightArm->getLocalTransform()->setScale(glm::vec3(0.1, 0.5, 0.1));
    enemyRightArm->getLocalTransform()->setPos(glm::vec3(0.13, 0.7, 0.3));
    enemyRightArm->getLocalTransform()->setRotation(M_PI / 2, glm::vec3(1, 0, 0));
    enemyObject->addComponent<DrawableComponent>(enemyRightArm);

    std::shared_ptr<DrawableComponent> enemyLeftArm = std::make_shared<DrawableComponent>();
    enemyLeftArm->setShape("cylinder");
    enemyLeftArm->setMaterial("zombieTexture");
    enemyLeftArm->getLocalTransform()->setScale(glm::vec3(0.1, 0.5, 0.1));
    enemyLeftArm->getLocalTransform()->setPos(glm::vec3(-0.13, 0.7, 0.3));
    enemyLeftArm->getLocalTransform()->setRotation(M_PI / 2, glm::vec3(1, 0, 0));
    enemyObject->addComponent<DrawableComponent>(enemyLeftArm);

    for (std::shared_ptr<DrawableComponent> drawComponent: enemyObject->getComponents<DrawableComponent>()) {
        drawComponent->getLocalTransform()->setModelMatrix(glm::rotate(glm::mat4(1), -(float)M_PI / 2, glm::vec3(0, 1, 0)) * drawComponent->getLocalTransform()->getModelMatrix());
    }

    m_gameWorld.lock()->addGameObject(enemyObject);
}

}
