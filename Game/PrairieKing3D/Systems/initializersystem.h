#pragma once

#include "Engine/system.h"
#include "Graphics/modeltransform.h"
#include "glm/glm.hpp"

namespace PrairieKing3D {

class InitializerSystem : public System
{
public:
    void initalizeGame();
    void addBullet(std::shared_ptr<ModelTransform> bulletModel);
    void addEnemy(glm::vec3 enemyPos, float enemyRotation);
    InitializerSystem();
    void loadGame(std::string fileName);
private:
    void initializeMap(std::string fileName);
    void initializePlayer();
    void addWall(glm::vec3 wallPos);
    void addFloor(std::shared_ptr<GameObject> floorObject, glm::vec3 floorPos);
    GameObjectID playerID;
    std::vector<GameObjectID> mapObjects;
};

}
