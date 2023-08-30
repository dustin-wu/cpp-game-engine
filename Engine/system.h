#pragma once

#include "Engine/gameobject.h"
#include "Engine/gameworld.h"
#include <typeindex>
#include <unordered_map>

class GameWorld;

class System
{
public:
    System(std::type_index requiredComponentType);
    System(std::vector<std::type_index> requiredComponentTypes);
    virtual ~System() {};
    std::type_index getRequiredComponentType();
    virtual void addGameObject(std::shared_ptr<GameObject> gameObject);
    virtual void removeGameObject(GameObjectID ID);
    virtual void reset(){};
    virtual bool hasGameObject(GameObjectID id) {return m_gameObjects.contains(id);}
    void setGameWorld(std::shared_ptr<GameWorld> gameWorld);

protected:
    std::unordered_map<GameObjectID, std::shared_ptr<GameObject>> m_gameObjects;
    std::weak_ptr<GameWorld> m_gameWorld;
//    std::type_index requiredComponentType_;
    std::vector<std::type_index> requiredComponentTypes_;

};

