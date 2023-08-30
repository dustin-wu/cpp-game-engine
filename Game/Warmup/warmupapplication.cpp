#include "warmupapplication.h"
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
#include "Game/Warmup/gamescreen.h"
#include "Game/Warmup/settingsscreen.h"
#include "Game/Warmup/titlescreen.h"
#include "Game/Warmup/hintsscreen.h"
#include "Graphics/global.h"
#include <iostream>

WarmUpApplication::WarmUpApplication(){}

void WarmUpApplication::initializeApp() {

    Global::graphics.addMaterial("grassTexture", "Resources/Images/grass.png");
    Global::graphics.addMaterial("obamaTexture", "Resources/Images/obama2.png");
    Global::graphics.addMaterial("trumpTexture", "Resources/Images/trump.jpg");

    m_gameWorld->addSystem<DrawSystem>();
    m_gameWorld->addSystem<CharacterControllerSystem>();
    m_gameWorld->addSystem<CameraSystem>();
    m_gameWorld->addSystem<PhysicsSystem>();
    m_gameWorld->addSystem<CollisionSystem>();

    std::shared_ptr<GameObject> playerObject = std::make_shared<GameObject>();
    std::shared_ptr<TransformComponent> playerTransform = std::make_shared<TransformComponent>();
    playerObject->addComponent<TransformComponent>(playerTransform);
    std::shared_ptr<CameraComponent> playerCamera = std::make_shared<CameraComponent>();
    playerObject->addComponent<CameraComponent>(playerCamera);
    std::shared_ptr<PlayerComponent> playerConfig = std::make_shared<PlayerComponent>();
    playerObject->addComponent<PlayerComponent>(playerConfig);
    std::shared_ptr<DrawableComponent> playerDrawable = std::make_shared<DrawableComponent>();
    playerDrawable->setShape("cylinder");
    playerDrawable->setMaterial("obamaTexture");
    playerDrawable->getLocalTransform()->setPos(glm::vec3(0, 0.5, 0));
    playerDrawable->getLocalTransform()->setRotation(M_PI / 2, glm::vec3(0, 1, 0));
    playerObject->addComponent<DrawableComponent>(playerDrawable);
    std::shared_ptr<RigidBodyComponent> playerRigidBody = std::make_shared<RigidBodyComponent>();
    playerObject->addComponent<RigidBodyComponent>(playerRigidBody);
    std::shared_ptr<CylinderCollisionComponent> playerCollider = std::make_shared<CylinderCollisionComponent>();
    playerObject->addComponent<CollisionComponent>(playerCollider);
    m_gameWorld->addGameObject(playerObject);

    std::shared_ptr<GameObject> otherObject = std::make_shared<GameObject>();
    std::shared_ptr<TransformComponent> otherTransform = std::make_shared<TransformComponent>();
    std::shared_ptr<ModelTransform> otherTransformDefault = std::make_shared<ModelTransform>();
    otherTransformDefault->setPos(glm::vec3(0, 0, 3));
    otherTransform->setDefaultTransform(otherTransformDefault);
    otherObject->addComponent<TransformComponent>(otherTransform);
    std::shared_ptr<DrawableComponent> otherDrawable = std::make_shared<DrawableComponent>();
    otherDrawable->setShape("cylinder");
    otherDrawable->setMaterial("trumpTexture");
    otherDrawable->getLocalTransform()->setPos(glm::vec3(0, 0.5, 0));
    otherDrawable->getLocalTransform()->setRotation(-M_PI / 2, glm::vec3(0, 1, 0));
    otherObject->addComponent<DrawableComponent>(otherDrawable);
    std::shared_ptr<RigidBodyComponent> otherRigidBody = std::make_shared<RigidBodyComponent>();
    otherRigidBody->setStationary(false);
    otherRigidBody->setVelocity(glm::vec3(0, 0, -0.1));
    otherObject->addComponent<RigidBodyComponent>(otherRigidBody);
    std::shared_ptr<CylinderCollisionComponent> otherCollider = std::make_shared<CylinderCollisionComponent>();
    otherObject->addComponent<CollisionComponent>(otherCollider);
    m_gameWorld->addGameObject(otherObject);

    int floorSize = 5;
    std::shared_ptr<GameObject> floorObject = std::make_shared<GameObject>();
    std::shared_ptr<TransformComponent> floorTransform = std::make_shared<TransformComponent>();
    floorObject->addComponent<TransformComponent>(floorTransform);
    glm::vec3 floorOffset = glm::vec3(-floorSize, 0, -floorSize);
    for (int i = -floorSize; i <= floorSize; i++) {
        for (int j = -floorSize; j <= floorSize; j++) {
            std::shared_ptr<DrawableComponent> floorDrawable = std::make_shared<DrawableComponent>();
            floorDrawable->setShape("quad");
            floorDrawable->setMaterial("grassTexture");
            floorDrawable->getLocalTransform()->translate(floorOffset);
            floorObject->addComponent<DrawableComponent>(floorDrawable);
            floorOffset += glm::vec3(1, 0, 0);
        }
        floorOffset = glm::vec3(-floorSize, 0, floorOffset.z);
        floorOffset += glm::vec3(0, 0, 1);
    }
    m_gameWorld->addGameObject(floorObject);

    std::shared_ptr<GameScreen> m_gamescreen = std::make_shared<GameScreen>();
    addScreen("game", m_gamescreen);

    std::shared_ptr<TitleScreen> m_titlescreen = std::make_shared<TitleScreen>();
    addScreen("title", m_titlescreen);

    std::shared_ptr<SettingsScreen> m_settingsscreen = std::make_shared<SettingsScreen>();
    addScreen("settings", m_settingsscreen);

    std::shared_ptr<HintsScreen> m_hintsscreen = std::make_shared<HintsScreen>();
    addScreen("hints", m_hintsscreen);

    setVisibleScreens({"title", "hints"});
    setActiveScreens({"title"});

}
