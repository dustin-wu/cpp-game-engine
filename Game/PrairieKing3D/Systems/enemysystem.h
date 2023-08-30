#pragma once

#include "Engine/system.h"
#include "glm/fwd.hpp"
#include <random>
#include <queue>

namespace PrairieKing3D {

class EnemySystem : public System
{
public:
    EnemySystem();
    void updateEnemies(float deltaTimef);
    void spawnEnemies(float deltaTimef);
    void setSpawnAreas(std::vector<glm::vec3> spawnAreas);
    void setSpawnAngles(std::vector<float> spawnAngles);
    void setWaves(std::queue<int> spawnCounts, std::queue<float> spawnCooldowns);
    std::string getWaveInfo();
    std::string getKillsInfo();
    void reset() override;
    bool won();
private:
    std::random_device rd;
    std::mt19937 gen;
    std::vector<glm::vec3> spawnAreas_;
    std::vector<float> spawnAngles_;
    std::queue<int> spawnCounts_;
    std::queue<float> spawnCooldowns_;
    std::queue<int> spawnCountsDefault_;
    std::queue<float> spawnCooldownsDefault_;
    float waveCooldown;
    int numKilled;
};

}
