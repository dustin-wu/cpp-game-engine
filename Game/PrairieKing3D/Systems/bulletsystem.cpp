#include "bulletsystem.h"
#include "Engine/Components/transformcomponent.h"
#include "Engine/Systems/collisionsystem.h"
#include "Game/PrairieKing3D/Components/bulletcomponent.h"
#include <typeinfo>
#include <glm/gtx/norm.hpp>
#include <unordered_set>

BulletSystem::BulletSystem()
    : System(typeid(BulletComponent))
{
}

void BulletSystem::reset() {
    auto allObjects = m_gameObjects;
    for (auto& [id, gameObject]: allObjects) {
        m_gameWorld.lock()->removeGameObject(id);
    }
}

void BulletSystem::trackBullets() {
    std::unordered_set<GameObjectID> toRemove;

    for (auto& [id, gameObject]: m_gameObjects) {
        glm::vec3 bulletEndPos = gameObject->getComponent<TransformComponent>()->getGlobalTransform()->getPos();
        if (glm::distance2(bulletStartPos[id], bulletEndPos) >= 1000 || bulletEndPos.y <= 0.001) {
            toRemove.insert(id);
        }
        if (m_gameWorld.lock()->getSystem<CollisionSystem>()->getCollisions()[id].size() > 0) {
            toRemove.insert(id);
        }
    }

    for (GameObjectID id: toRemove) {
        m_gameWorld.lock()->removeGameObject(id);
    }
}

void BulletSystem::addGameObject(std::shared_ptr<GameObject> gameObject) {
    if (gameObject->hasComponentOfType(typeid(BulletComponent))) {
        m_gameObjects[gameObject->ID()] = gameObject;
        bulletStartPos[gameObject->ID()] = gameObject->getComponent<TransformComponent>()->getGlobalTransform()->getPos();
    }
}

void BulletSystem::removeGameObject(GameObjectID ID) {
    bulletStartPos.erase(ID);
    m_gameObjects.erase(ID);
}
