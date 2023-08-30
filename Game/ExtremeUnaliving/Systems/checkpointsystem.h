#pragma once


#include "Engine/Systems/particlesystem.h"
#include "Engine/system.h"
#include "Game/ExtremeUnaliving/Components/checkpointcomponent.h"


namespace ExtremeUnaliving {

class CheckpointSystem : public System
{
public:
    CheckpointSystem();
    void checkCheckpoints(float deltaTime);
    void resetCheckpoints();
    glm::vec3 getLatestCheckpointPos();
    glm::mat4 getLatestCheckpointRot();

private:
    std::shared_ptr<CheckpointComponent> latestCheckpoint_;
    std::unordered_map<GameObjectID, std::shared_ptr<GameObject>> originalCheckpoints_;
    bool originalSet = false;
    std::shared_ptr<ParticleSystem> m_particleSystem;
};

}
