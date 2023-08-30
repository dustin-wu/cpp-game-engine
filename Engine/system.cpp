#include "system.h"
#include <iostream>

System::System(std::type_index requiredComponentType)
//    : requiredComponentType_(requiredComponentType)
{
    requiredComponentTypes_.push_back(requiredComponentType);
}

System::System(std::vector<std::type_index> requiredComponentTypes)
{
    requiredComponentTypes_ = requiredComponentTypes;
}

std::type_index System::getRequiredComponentType() {
    return requiredComponentTypes_[0];
}

void System::addGameObject(std::shared_ptr<GameObject> gameObject) {
    bool shouldAdd = false;
    for (std::type_index requiredComponentType: requiredComponentTypes_) {
        if (gameObject->hasComponentOfType(requiredComponentType)) {
            shouldAdd = true;
            break;
        }
    }
    if (shouldAdd) {
        m_gameObjects[gameObject->ID()] = gameObject;
    }
}

void System::removeGameObject(GameObjectID ID) {
    m_gameObjects.erase(ID);
}

void System::setGameWorld(std::shared_ptr<GameWorld> gameWorld) {
    m_gameWorld = gameWorld;
}
