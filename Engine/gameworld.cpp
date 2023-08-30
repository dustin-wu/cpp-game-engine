#include "gameworld.h"
#include <iostream>



GameWorld::GameWorld()
{
}

void GameWorld::addGameObject(std::shared_ptr<GameObject> gameObject) {
    m_objects[gameObject->ID()] = gameObject;
    for (auto& [key, system]: m_systems) {
        system->addGameObject(gameObject);
    }
}


void GameWorld::removeGameObject(GameObjectID ID) {
    if (!m_objects.contains(ID)) return;
    std::shared_ptr<GameObject> gameObject = m_objects[ID];

    for (auto& [key, system]: m_systems) {
        std::type_index requiredType = system->getRequiredComponentType();
        if (gameObject->hasComponentOfType(requiredType)) {
            system->removeGameObject(ID);
        }
    }

    m_objects.erase(ID);
}

std::shared_ptr<GameObject> GameWorld::getGameObject(GameObjectID ID) {
    return m_objects[ID];
}

bool GameWorld::hasGameObject(GameObjectID ID) {
    return m_objects.contains(ID);
}


void GameWorld::reset() {
    for (auto& [key, gameObject]: m_objects) {
        for (auto& [componentKey, componentVector]: gameObject->getAllComponents()) {
            for (auto& component: componentVector) {
                component->reset();
            }
        }
    }

    for (auto& [key, system]: m_systems) {
        system->reset();
    }
}
