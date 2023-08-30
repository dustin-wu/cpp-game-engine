#include "gameobject.h"

GameObject::GameObject()
    : ID_(nextID_)
{
    nextID_++; // Constructor increments static variable so ID will always update with new GameObjects
}

GameObjectID GameObject::nextID_ = 0; // Initialize the static nextID to 0

GameObjectID GameObject::ID() {
    return ID_;
}
