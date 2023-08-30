#include "initializersystem.h"
#include "Engine/Components/CollisionComponents/cylindercollisioncomponent.h"
#include "Engine/Components/CollisionComponents/ellipsoidcollisioncomponent.h"
#include "Engine/Components/CollisionComponents/trianglecollisioncomponent.h"
#include "Engine/Components/cameracomponent.h"
#include "Engine/Components/drawablecomponent.h"
#include "Engine/Components/nocomponent.h"
#include "Engine/Components/playercomponent.h"
#include "Engine/Components/rigidbodycomponent.h"
#include "Engine/Components/transformcomponent.h"
#include "Engine/Systems/aisystem.h"
#include "Engine/Systems/camerasystem.h"
#include "Engine/Systems/charactercontrollersystem.h"
#include "Engine/Systems/collisionsystem.h"
#include "Engine/Systems/drawsystem.h"
#include "Engine/Systems/navmeshsystem.h"
#include "Engine/Systems/physicssystem.h"
#include "Engine/Systems/uniformgridsystem.h"
#include "Graphics/global.h"

InitializerSystem::InitializerSystem()
    : System(typeid(NoComponent))
{
    Global::graphics.addMaterial("placeholder", "Resources/Images/placeholder.jpg");
}

void InitializerSystem::InitializeGame() {
    std::shared_ptr<GameWorld> gameWorld = m_gameWorld.lock();
    gameWorld->addSystem<DrawSystem>();
    gameWorld->addSystem<CharacterControllerSystem>();
    gameWorld->addSystem<CameraSystem>();
    gameWorld->addSystem<PhysicsSystem>();
    gameWorld->addSystem<CollisionSystem>();
    gameWorld->addSystem<UniformGridSystem>();
    gameWorld->addSystem<NavMeshSystem>();
    gameWorld->addSystem<AISystem>();
}

void InitializerSystem::InitializePlayer() {
    std::shared_ptr<GameWorld> gameWorld = m_gameWorld.lock();
    std::shared_ptr<ModelTransform> basePlayerTransform = std::make_shared<ModelTransform>();
    std::shared_ptr<GameObject> basePlayer = CreateBasePlayerObject(basePlayerTransform);
    gameWorld->addGameObject(basePlayer);
}

std::shared_ptr<GameObject> InitializerSystem::CreateBasePlayerObject(std::shared_ptr<ModelTransform> model) {
    std::shared_ptr<GameObject> playerObject = std::make_shared<GameObject>();
    playerID = playerObject->ID();
    m_gameWorld.lock()->setPlayerID(playerID);
    std::shared_ptr<TransformComponent> playerTransform = std::make_shared<TransformComponent>();
    playerTransform->setDefaultTransform(model);
    playerObject->addComponent<TransformComponent>(playerTransform);
    std::shared_ptr<CameraComponent> playerCamera = std::make_shared<CameraComponent>();
    playerObject->addComponent<CameraComponent>(playerCamera);
    std::shared_ptr<PlayerComponent> playerConfig = std::make_shared<PlayerComponent>();
    playerConfig->setEyeHeight(1);
    playerObject->addComponent<PlayerComponent>(playerConfig);
    std::shared_ptr<RigidBodyComponent> playerRigidBody = std::make_shared<RigidBodyComponent>();
    playerObject->addComponent<RigidBodyComponent>(playerRigidBody);
    std::shared_ptr<CylinderCollisionComponent> playerCylinderCollider = std::make_shared<CylinderCollisionComponent>();
    playerObject->addComponent<CylinderCollisionComponent>(playerCylinderCollider);
    std::shared_ptr<EllipsoidCollisionComponent> playerEllipsoidCollider = std::make_shared<EllipsoidCollisionComponent>();
    playerObject->addComponent<EllipsoidCollisionComponent>(playerEllipsoidCollider);
    std::shared_ptr<DrawableComponent> playerDrawable = std::make_shared<DrawableComponent>();
    playerDrawable->setShape("sphere");
    playerDrawable->setMaterial("placeholder");
    playerDrawable->getLocalTransform()->setPos(glm::vec3(0, 0.5, 0));
    playerObject->addComponent<DrawableComponent>(playerDrawable);
    return playerObject;
}

std::shared_ptr<GameObject> InitializerSystem::CreateBaseKineticObject(std::shared_ptr<ModelTransform> model) {
    std::shared_ptr<GameObject> kineticObject = std::make_shared<GameObject>();
    std::shared_ptr<TransformComponent> kineticTransform = std::make_shared<TransformComponent>();
    kineticTransform->setDefaultTransform(model);
    kineticObject->addComponent<TransformComponent>(kineticTransform);
    std::shared_ptr<RigidBodyComponent> kineticRigidBody = std::make_shared<RigidBodyComponent>();
    kineticObject->addComponent<RigidBodyComponent>(kineticRigidBody);
    std::shared_ptr<CylinderCollisionComponent> kineticCylinderCollider = std::make_shared<CylinderCollisionComponent>();
    kineticObject->addComponent<CylinderCollisionComponent>(kineticCylinderCollider);
    std::shared_ptr<EllipsoidCollisionComponent> kineticEllipsoidCollider = std::make_shared<EllipsoidCollisionComponent>();
    kineticObject->addComponent<EllipsoidCollisionComponent>(kineticEllipsoidCollider);
    std::shared_ptr<DrawableComponent> kineticDrawable = std::make_shared<DrawableComponent>();
    kineticDrawable->setShape("cylinder");
    kineticDrawable->setMaterial("placeholder");
    kineticDrawable->getLocalTransform()->setPos(glm::vec3(0, 0.5, 0));
    kineticObject->addComponent<DrawableComponent>(kineticDrawable);
    return kineticObject;
}

std::shared_ptr<GameObject> InitializerSystem::CreateBaseStaticObject(std::shared_ptr<ModelTransform> model, std::vector<glm::vec3>& vertices, std::string shape) {
    std::shared_ptr<GameObject> staticObject = std::make_shared<GameObject>();
    std::shared_ptr<TransformComponent> staticTransform = std::make_shared<TransformComponent>();
    staticTransform->setDefaultTransform(model);
    staticObject->addComponent<TransformComponent>(staticTransform);
    std::shared_ptr<RigidBodyComponent> staticRigidBody = std::make_shared<RigidBodyComponent>();
    staticRigidBody->setStationary(true);
    staticObject->addComponent<RigidBodyComponent>(staticRigidBody);
    std::shared_ptr<TriangleCollisionComponent> staticCollider = std::make_shared<TriangleCollisionComponent>(model, vertices);
    staticObject->addComponent<TriangleCollisionComponent>(staticCollider);
    std::shared_ptr<DrawableComponent> staticDrawable = std::make_shared<DrawableComponent>();
    staticDrawable->setShape(shape);
    staticDrawable->setMaterial("placeholder");
    staticObject->addComponent<DrawableComponent>(staticDrawable);
    return staticObject;
}
