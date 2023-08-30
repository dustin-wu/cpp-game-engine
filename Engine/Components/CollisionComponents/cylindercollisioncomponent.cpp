#include "cylindercollisioncomponent.h"

CylinderCollisionComponent::CylinderCollisionComponent()
    : CollisionComponent(typeid(CylinderCollisionComponent))
    , localPos_(glm::vec3(0, 0.5, 0))
    , radius_(0.5)
    , height_(1),
      physics_(true)
{

}

CylinderCollisionComponent::CylinderCollisionComponent(bool physics)
    : CollisionComponent(typeid(CylinderCollisionComponent))
    , localPos_(glm::vec3(0, 0.5, 0))
    , radius_(0.5)
    , height_(1),
      physics_(physics)
{

}




void CylinderCollisionComponent::setRadius(float radius) {
    radius_ = radius;
}

void CylinderCollisionComponent::setHeight(float height) {
    height_ = height;
}

void CylinderCollisionComponent::setLocalPos(glm::vec3 localPos) {
    localPos_ = localPos;
}

float CylinderCollisionComponent::getRadius() {
    return radius_;
}

float CylinderCollisionComponent::getHeight() {
    return height_;
}

glm::vec3 CylinderCollisionComponent::getLocalPos() {
    return localPos_;
}

void CylinderCollisionComponent::makeImmovable() {
    immovable_ = true;
}
