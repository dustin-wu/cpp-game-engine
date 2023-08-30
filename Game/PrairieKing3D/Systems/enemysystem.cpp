#include "enemysystem.h"
#include "Engine/Components/playercomponent.h"
#include "Engine/Components/transformcomponent.h"
#include "Engine/Systems/collisionsystem.h"
#include "Game/PrairieKing3D/Components/bulletcomponent.h"
#include "Game/PrairieKing3D/Components/enemycomponent.h"
#include "Game/PrairieKing3D/Systems/healthsystem.h"
#include "Game/PrairieKing3D/Systems/initializersystem.h"
#include <unordered_set>

namespace PrairieKing3D {

EnemySystem::EnemySystem()
    : System(typeid(EnemyComponent))
    , gen(rd())
    , waveCooldown(5)
{
    reset();
}

bool EnemySystem::won() {
    return (spawnCounts_.size() == 0) && (m_gameObjects.size() == 0);
}

void EnemySystem::reset() {
    waveCooldown = 5;
    spawnCooldowns_ = spawnCooldownsDefault_;
    spawnCounts_ = spawnCountsDefault_;
    auto allObjects = m_gameObjects;
    for (auto& [id, gameObject]: allObjects) {
        m_gameWorld.lock()->removeGameObject(id);
    }
    numKilled = 0;
}

void EnemySystem::setWaves(std::queue<int> spawnCounts, std::queue<float> spawnCooldowns) {
    spawnCounts_ = spawnCounts;
    spawnCountsDefault_ = spawnCounts;
    spawnCooldowns_ = spawnCooldowns;
    spawnCooldownsDefault_ = spawnCooldowns;
}

void EnemySystem::setSpawnAreas(std::vector<glm::vec3> spawnAreas) {
    spawnAreas_ = spawnAreas;
}

void EnemySystem::setSpawnAngles(std::vector<float> spawnAngles) {
    spawnAngles_ = spawnAngles;
}

void EnemySystem::spawnEnemies(float deltaTimef) {
    if (spawnAreas_.size() == 0) return; // if no spawn areas, can't spawn enemies
    if (spawnCounts_.size() == 0) return; // no more waves

     std::uniform_int_distribution<> dist(0, spawnAreas_.size() - 1);

    if (waveCooldown <= 0) {
        int numEnemies = spawnCounts_.front();
        spawnCounts_.pop();
        float cooldown = spawnCooldowns_.front();
        spawnCooldowns_.pop();
        for (int i = 0; i < numEnemies; i++) {
            int index = dist(gen);
            glm::vec3 spawnPos = spawnAreas_[index];
            float spawnAngle = spawnAngles_[index];
            m_gameWorld.lock()->getSystem<PrairieKing3D::InitializerSystem>()->addEnemy(spawnPos, spawnAngle);
        }

        waveCooldown = cooldown;
    } else {
        waveCooldown -= deltaTimef;
    }
}

std::string EnemySystem::getWaveInfo() {
    if (spawnCounts_.size() > 0) {
        return "Next wave in " + std::to_string((int) waveCooldown + 1) + " seconds";
    } else {
        return "Clear this last wave to win!";
    }
}

std::string EnemySystem::getKillsInfo() {
    return "Enemies Unalived: " + std::to_string(numKilled);
}


void EnemySystem::updateEnemies(float deltaTimef) {
    std::unordered_set<GameObjectID> toRemove;

    for (auto& [id, gameObject]: m_gameObjects) {
        std::shared_ptr<EnemyComponent> enemyConfig = gameObject->getComponent<EnemyComponent>();
        if (enemyConfig->getAttackCooldown() > 0)
            enemyConfig->setAttackCooldown(enemyConfig->getAttackCooldown() - deltaTimef);

        std::vector<CollisionInfo> collisions = m_gameWorld.lock()->getSystem<CollisionSystem>()->getCollisions()[id];
        bool hitBullet = false;
        for (CollisionInfo collision: collisions) {
            if (m_gameWorld.lock()->getGameObject(collision.otherID)->hasComponentOfType(typeid(BulletComponent))) {
                hitBullet = true;
                break;
            }
            if (collision.otherID == enemyConfig->getTarget()) {
                if (enemyConfig->getAttackCooldown() <= 0) {
                    m_gameWorld.lock()->getSystem<HealthSystem>()->takeDamage(10);
                    enemyConfig->setAttackCooldown(2.f);
                }
            }
        }

        if (hitBullet) {
            toRemove.insert(id);
        } else {
            std::shared_ptr<ModelTransform> enemyModel = gameObject->getComponent<TransformComponent>()->getGlobalTransform();
            if (enemyModel->getPos().y < 0) enemyModel->setPos(glm::vec3(enemyModel->getPos().x, 0, enemyModel->getPos().z));
            std::shared_ptr<GameObject> targetObj = m_gameWorld.lock()->getGameObject(enemyConfig->getTarget());
            glm::vec3 targetPos = targetObj->getComponent<TransformComponent>()->getGlobalTransform()->getPos();
            glm::vec3 directionToTarget = glm::normalize(targetPos - enemyModel->getPos());
            glm::vec3 forward = glm::vec3(enemyModel->getRotation() * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
            float dot = glm::dot(directionToTarget, forward);
            float angle = glm::acos(dot);
            if (glm::sign(dot) < 0) {
                angle *= -1;
            }
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle * enemyConfig->getTurnSpeed() * deltaTimef, glm::vec3(0, 1, 0));
            enemyModel->rotate(rotation);
            enemyModel->translate(deltaTimef * enemyConfig->getSpeed() * glm::vec3(directionToTarget.x, 0, directionToTarget.z));
        }
    }

    for (GameObjectID id: toRemove) {
        m_gameWorld.lock()->removeGameObject(id);
        numKilled++;
    }

}

}
