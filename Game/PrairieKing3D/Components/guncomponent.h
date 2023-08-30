#pragma once

#include "Engine/gameobject.h"

class GunComponent : public Component
{
public:
    GunComponent(GameObjectID playerID);
    GameObjectID getPlayerID();
private:
    GameObjectID playerID_;

};

