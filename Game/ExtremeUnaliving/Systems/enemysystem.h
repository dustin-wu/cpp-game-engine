#pragma once

#include "Engine/Systems/particlesystem.h"
#include "Engine/system.h"
#include "glm/fwd.hpp"
#include <random>
#include <queue>
#include <unordered_set>

namespace ExtremeUnaliving {

class EnemySystem : public System
{
public:
    EnemySystem();
    void updateEnemies(float deltaTimef);
    void reset() override;
    void cullEnemies();
    void takeDamage(GameObjectID enemyID, glm::vec3 hitPos);
    bool enemiesRemaining();
    std::vector<GameObjectID> getEnemies();
private:
    std::unordered_set<GameObjectID> toRemove;
    std::shared_ptr<ParticleSystem> m_bloodParticleSystem;
    std::shared_ptr<ParticleSystem> m_superBloodParticleSystem;

};

}
