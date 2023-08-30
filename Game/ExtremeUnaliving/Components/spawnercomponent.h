#pragma once

#include "Engine/component.h"
#include "glm/glm.hpp"
#include <queue>
#include <vector>

namespace ExtremeUnaliving {

struct SpawnInfo {
    glm::vec3 position;
    int enemyType;
};


class SpawnerComponent : public Component
{
public:
    SpawnerComponent(glm::vec3 triggerPosition);
    glm::vec3 getTriggerPosition() { return triggerPosition_; }
    void insertWave(std::vector<SpawnInfo> wave);
    std::vector<SpawnInfo> getNextWave();
    bool noEnemies() { return waves_.empty(); }
    void reset() override;

private:
    glm::vec3 triggerPosition_;
    std::queue<std::vector<SpawnInfo>> waves_;
    std::queue<std::vector<SpawnInfo>> wavesOriginal_;
};

}

