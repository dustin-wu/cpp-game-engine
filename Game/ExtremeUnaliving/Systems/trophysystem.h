#pragma once

#include "Engine/system.h"
namespace ExtremeUnaliving {

class TrophySystem : public System
{
public:
    TrophySystem();
    bool checkTrophy(float deltaTime);
};


}
