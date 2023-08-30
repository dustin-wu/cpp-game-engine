#pragma once

#include "Engine/gameobject.h"
#include "Engine/system.h"
#include <unordered_map>

class System;

class GameWorld : public std::enable_shared_from_this<GameWorld>
{
public:
    GameWorld();

    void addGameObject(std::shared_ptr<GameObject> gameObject);
    void removeGameObject(GameObjectID ID);
    std::shared_ptr<GameObject> getGameObject(GameObjectID ID);
    bool hasGameObject(GameObjectID ID);

    template <class T>
    void addSystem() {
        std::shared_ptr<T> system = std::make_shared<T>();
        system->setGameWorld(shared_from_this());
        m_systems[typeid(T)] = system;
    }

    template <class T>
    void removeSystem() {
        m_systems.erase(typeid(T));
    }

    template <class T>
    std::shared_ptr<T> getSystem() {
        return dynamic_pointer_cast<T>(m_systems[typeid(T)]);
    }

    void reset();

    void setPlayerID(GameObjectID playerID) {playerID_ = playerID;}
    GameObjectID getPlayerID() {return playerID_;}


private:
    std::unordered_map<GameObjectID, std::shared_ptr<GameObject>> m_objects;
    std::unordered_map<std::type_index, std::shared_ptr<System>> m_systems;
    GameObjectID playerID_;
};

