#include "guncomponent.h"

GunComponent::GunComponent(GameObjectID playerID)
    : playerID_(playerID)
{

}

GameObjectID GunComponent::getPlayerID() {
    return playerID_;
}
