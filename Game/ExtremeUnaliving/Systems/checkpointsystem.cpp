#include "checkpointsystem.h"
#include "Engine/Components/drawablecomponent.h"
#include "Engine/Components/playercomponent.h"
#include "Engine/Components/transformcomponent.h"
#include "Engine/Systems/collisionsystem.h"

namespace ExtremeUnaliving {

CheckpointSystem::CheckpointSystem()
    : System(typeid(CheckpointComponent))
{
    ParticleTexture pt = ParticleTexture("star", 5);
    m_particleSystem = std::make_shared<ParticleSystem>(pt, 1.f, 3.f, 1.f, 1.f, 0.25f);
    latestCheckpoint_ = nullptr;

}

glm::vec3 CheckpointSystem::getLatestCheckpointPos() {
    if (latestCheckpoint_ == nullptr) return glm::vec3(0, 1, 0); // default start
    return latestCheckpoint_->getPosition() + glm::vec3(0, 1, 0);
}

glm::mat4 CheckpointSystem::getLatestCheckpointRot() {
    if (latestCheckpoint_ == nullptr) return glm::rotate(glm::mat4(1), -float(M_PI/2), glm::vec3(0,1,0)); // default rotation
    return latestCheckpoint_->getRotation();
}

void CheckpointSystem::checkCheckpoints(float deltaTime) {
    for (auto& [id, gameObject]: m_gameObjects) {
        glm::mat4 curRot = gameObject->getComponent<DrawableComponent>()->getLocalTransform()->getRotation();
        gameObject->getComponent<DrawableComponent>()->getLocalTransform()->setRotation(glm::rotate(curRot, (float)M_PI / 4.f * deltaTime, glm::vec3(0,1,0)));

        glm::vec3 checkPos = gameObject->getComponent<CheckpointComponent>()->getPosition();
        glm::vec3 playerPos = m_gameWorld.lock()->getGameObject(m_gameWorld.lock()->getPlayerID())->getComponent<TransformComponent>()->getGlobalTransform()->getPos();
        if (glm::distance(playerPos, checkPos) <= 2) {
            latestCheckpoint_ = gameObject->getComponent<CheckpointComponent>();

            m_gameWorld.lock()->removeGameObject(id);
            m_particleSystem->generateParticles(checkPos + glm::vec3(0, 1, 0), 7);
            return;
        }
    }
}

void CheckpointSystem::resetCheckpoints() {
    if (!originalSet) {
        originalSet = true;
        originalCheckpoints_ = m_gameObjects;
        return;
    }

    // Remove any checkpoints that haven't been reached before adding them all back
    for (auto& [id, gameObject]: originalCheckpoints_) {
        if (m_gameWorld.lock()->hasGameObject(id))
            m_gameWorld.lock()->removeGameObject(id);
    }

    for (auto& [id, gameObject]: originalCheckpoints_) {
        m_gameWorld.lock()->addGameObject(gameObject);
    }
    latestCheckpoint_ = nullptr;
}


}
