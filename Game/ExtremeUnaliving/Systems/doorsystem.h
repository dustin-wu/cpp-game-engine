#pragma once

#include "Engine/system.h"


namespace ExtremeUnaliving {


class DoorSystem : public System
{
public:
    DoorSystem();
    void checkDoors();
    void resetDoors();

private:
    bool originalSet = false;
    std::unordered_map<GameObjectID, std::shared_ptr<GameObject>> originalDoors_;

};


}

