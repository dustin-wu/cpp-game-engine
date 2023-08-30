#include "enemysystem.h"
#include "Engine/Components/CollisionComponents/cylindercollisioncomponent.h"
#include "Engine/Components/playercomponent.h"
#include "Engine/Components/transformcomponent.h"
#include "Engine/Systems/collisionsystem.h"
#include "Game/ExtremeUnaliving/Components/bulletcomponent.h"
#include "Game/ExtremeUnaliving/Components/enemycomponent.h"
#include "Game/ExtremeUnaliving/ShopHandler.h"
#include "Game/ExtremeUnaliving/Systems/bulletsystem.h"
#include "Game/ExtremeUnaliving/Systems/healthsystem.h"
#include "Game/ExtremeUnaliving/Systems/gameinitializersystem.h"
#include <unordered_set>
#include <glm/gtc/random.hpp>

namespace ExtremeUnaliving {

EnemySystem::EnemySystem()
    : System(typeid(EnemyComponent))
{
    reset();
    ParticleTexture pt = ParticleTexture("bloodsparks", 1);
    m_bloodParticleSystem = std::make_shared<ParticleSystem>(pt, 1.f, 3.f, 1.f, 1.f, 0.1f);
    m_superBloodParticleSystem = std::make_shared<ParticleSystem>(pt, 1.f, 5.f, 1.f, 1.5f, 0.1f);
    m_superBloodParticleSystem->setScaleError(0.5);
}

void EnemySystem::reset() {
    auto allObjects = m_gameObjects;
    for (auto& [id, gameObject]: allObjects) {
        m_gameWorld.lock()->removeGameObject(id);
    }

}

void EnemySystem::updateEnemies(float deltaTimef) {

}

bool EnemySystem::enemiesRemaining() {
    return m_gameObjects.size() > 0;
}

void EnemySystem::takeDamage(GameObjectID enemyID, glm::vec3 hitPos) {
    std::shared_ptr<EnemyComponent> enemyConfig = m_gameObjects[enemyID]->getComponent<EnemyComponent>();
    float enemyY = m_gameObjects[enemyID]->getComponent<TransformComponent>()->getGlobalTransform()->getPos().y;
    float enemyHeight = m_gameObjects[enemyID]->getComponent<CylinderCollisionComponent>()->getHeight();
    int enemyType = enemyConfig->enemyType;
    int damage = 0;
    if (ShopHandling::handler.itemBought(4)) {
        damage++;
    }

    float normalNum = 4.f;
    float normalDenom = 5.f;
    float largeNum = 7.f;
    float largeDenom = 8.f;

    if (ShopHandling::handler.itemBought(3)) {
        normalNum-=2;
        normalDenom-=2;
        largeNum-=2;
        largeDenom-=2;
    }

    if (enemyType != 2 && hitPos.y >= enemyY + normalNum * enemyHeight / normalDenom) {
        m_superBloodParticleSystem->generateParticles(hitPos, 10);
        damage += 3;
        ShopHandling::handler.numSteel++;
    }
    else if (enemyType == 2 && hitPos.y >= enemyY + largeNum * enemyHeight / largeDenom) {
        m_superBloodParticleSystem->generateParticles(hitPos, 10);
        damage += 3;
        ShopHandling::handler.numSteel++;
    }
    else {
        m_bloodParticleSystem->generateParticles(hitPos, 3);
        damage++;
    }

     enemyConfig->setHP(enemyConfig->getHP() - damage);

    if (enemyConfig->getHP() <= 0) {
        toRemove.insert(enemyID);
    }
}

void EnemySystem::cullEnemies() {
    for (GameObjectID id: toRemove) {
        m_gameWorld.lock()->removeGameObject(id);
    }
}

std::vector<GameObjectID> EnemySystem::getEnemies() {
    std::vector<GameObjectID> output;
    for (auto& [id, gameObject]: m_gameObjects) {
        output.push_back(id);
    }
    return output;
}

}
