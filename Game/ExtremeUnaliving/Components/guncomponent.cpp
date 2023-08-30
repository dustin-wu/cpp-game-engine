#include "guncomponent.h"

namespace ExtremeUnaliving {

GunComponent::GunComponent(GameObjectID playerID)
    : playerID_(playerID)
{

}

GameObjectID GunComponent::getPlayerID() {
    return playerID_;
}

}
