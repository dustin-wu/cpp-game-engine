#pragma once


#include "Engine/system.h"
#include "Game/ExtremeUnaliving/Components/spawnercomponent.h"
#include "glm/glm.hpp"
#include <unordered_set>

namespace ExtremeUnaliving {

class SpawnerSystem : public System
{
public:
    SpawnerSystem();
    void attemptToSpawn();
    void reset() override;
    bool spawnerFinished(GameObjectID spawnerID) { return finishedSpawners_.contains(spawnerID); }

private:
    GameObjectID activeID;
    std::shared_ptr<SpawnerComponent> activeSpawner_;
    std::unordered_set<GameObjectID> finishedSpawners_;
};



}
