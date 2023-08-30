#include "collisioncomponent.h"

CollisionComponent::CollisionComponent(std::type_index colliderType)
    : colliderType_(colliderType)
{

}

std::type_index CollisionComponent::getColliderType() {
    return colliderType_;
}
