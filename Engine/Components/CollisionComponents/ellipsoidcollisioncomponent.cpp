#include "ellipsoidcollisioncomponent.h"

EllipsoidCollisionComponent::EllipsoidCollisionComponent()
    : CollisionComponent(typeid(EllipsoidCollisionComponent))
    , localPos_(glm::vec3(0, 0.5, 0))
    , radii_(glm::vec3(0.5, 0.5, 0.5))
{

}

void EllipsoidCollisionComponent::setRadii(glm::vec3 radii) {
    radii_ = radii;
}

void EllipsoidCollisionComponent::setLocalPos(glm::vec3 localPos) {
    localPos_ = localPos;
}


glm::vec3 EllipsoidCollisionComponent::getLocalPos() {
    return localPos_;
}

glm::vec3 EllipsoidCollisionComponent::getRadii() {
    return radii_;
}
