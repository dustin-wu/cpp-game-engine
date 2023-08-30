#include "doorsystem.h"
#include "Engine/Components/CollisionComponents/trianglecollisioncomponent.h"
#include "Engine/Components/drawablecomponent.h"
#include "Engine/Components/transformcomponent.h"
#include "Game/ExtremeUnaliving/Components/doorcomponent.h"
#include "Game/ExtremeUnaliving/Systems/spawnersystem.h"
namespace ExtremeUnaliving {

DoorSystem::DoorSystem()
    : System(typeid(DoorComponent))
{

}

void DoorSystem::checkDoors() {
    for (auto& [id, gameObject]: m_gameObjects) {
        GameObjectID lockedBehind = gameObject->getComponent<DoorComponent>()->getLockedBehind();

        if (m_gameWorld.lock()->getSystem<SpawnerSystem>()->spawnerFinished(lockedBehind)) {
            m_gameWorld.lock()->removeGameObject(id);
            return;
        }
    }
}

void DoorSystem::resetDoors() {
    if (!originalSet) {
        originalSet = true;
        originalDoors_ = m_gameObjects;
        return;
    }

    for (auto& [id, gameObject]: originalDoors_) {
        if (m_gameWorld.lock()->hasGameObject(id))
            m_gameWorld.lock()->removeGameObject(id);
    }

    for (auto& [id, gameObject]: originalDoors_) {
        m_gameWorld.lock()->addGameObject(gameObject);
        std::cout << gameObject->getComponent<TransformComponent>()->getGlobalTransform()->getPos().x << std::endl;
    }
}


}
