#include "spawnersystem.h"
#include "Engine/Components/transformcomponent.h"
#include "Game/ExtremeUnaliving/Components/spawnercomponent.h"
#include "Game/ExtremeUnaliving/Systems/enemysystem.h"
#include "Game/ExtremeUnaliving/Systems/gameinitializersystem.h"

namespace ExtremeUnaliving {

SpawnerSystem::SpawnerSystem()
    : System(typeid(SpawnerComponent))
{
    activeSpawner_ = nullptr;
}

void SpawnerSystem::reset() {
    activeSpawner_ = nullptr;
    finishedSpawners_.clear();
}


void SpawnerSystem::attemptToSpawn() {
    bool enemiesLeft = m_gameWorld.lock()->getSystem<EnemySystem>()->enemiesRemaining();
    if (enemiesLeft) return; // dont spawn more enemies when a wave is stil alive

    glm::vec3 playerPos = m_gameWorld.lock()->getGameObject(m_gameWorld.lock()->getPlayerID())->getComponent<TransformComponent>()->getGlobalTransform()->getPos();

    if (activeSpawner_ == nullptr) {

        for (auto& [id, gameObject]: m_gameObjects) {
            std::shared_ptr<SpawnerComponent> spawner = gameObject->getComponent<SpawnerComponent>();
            bool closeEnough = (glm::distance(playerPos, spawner->getTriggerPosition()) <= 15.f);
//            std::cout << closeEnough << std::endl;
            if (closeEnough && !spawner->noEnemies()) {
                activeSpawner_ = spawner;
                activeID = id;
                break;
            }
        }
    }

    if (activeSpawner_ == nullptr) return; // no current spawner

    if (!activeSpawner_->noEnemies()) {
        std::vector<SpawnInfo> spawnInfos = activeSpawner_->getNextWave();
        for (const SpawnInfo& spawnInfo: spawnInfos) {
            m_gameWorld.lock()->getSystem<GameInitializerSystem>()->addEnemy(spawnInfo.position, spawnInfo.enemyType);
        }
    }

    if (activeSpawner_->noEnemies() && !m_gameWorld.lock()->getSystem<EnemySystem>()->enemiesRemaining()) {
        activeSpawner_ = nullptr; // retire the spawner after it has no more enemies
        finishedSpawners_.insert(activeID);
    }

}

}
