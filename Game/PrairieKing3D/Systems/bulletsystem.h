#pragma once

#include "Engine/system.h"
#include "glm/fwd.hpp"

class BulletSystem : public System
{
public:
    BulletSystem();
    void trackBullets();
    void addGameObject(std::shared_ptr<GameObject> gameObject) override;
    void removeGameObject(GameObjectID ID) override;
    void reset() override;
    std::unordered_map<GameObjectID, glm::vec3> bulletStartPos;
};

