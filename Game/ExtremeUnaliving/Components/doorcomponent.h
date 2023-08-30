#pragma once

#include "Engine/gameobject.h"

namespace ExtremeUnaliving {

class DoorComponent : public Component
{
public:
    DoorComponent(GameObjectID lockedBehind);
    GameObjectID getLockedBehind(){ return lockedBehind_; }


private:
    GameObjectID lockedBehind_;
};

}
