#include "rigidbodycomponent.h"

RigidBodyComponent::RigidBodyComponent()
    : stationary_(false)
    , affectedByGravity_(true)
    , gravity_(9.8)
    , velocity_(glm::vec3(0))
    , acceleration_(glm::vec3(0))
{

}

void RigidBodyComponent::setVelocity(glm::vec3 velocity) {
    velocity_ = velocity;
}

void RigidBodyComponent::setAcceleration(glm::vec3 acceleration) {
    acceleration_ = acceleration;
}

void RigidBodyComponent::setStationary(bool stationary) {
    stationary_ = stationary;
}

void RigidBodyComponent::setGravity(float gravity) {
    gravity_ = gravity;
}

void RigidBodyComponent::setAffectedByGravity(bool affectedByGravity) {
    affectedByGravity_ = affectedByGravity;
}

bool RigidBodyComponent::getStationary() {
    return stationary_;
}

bool RigidBodyComponent::getAffectedByGravity() {
    return affectedByGravity_;
}

float RigidBodyComponent::getGravity() {
    return gravity_;
}

glm::vec3 RigidBodyComponent::getVelocity() {
    return velocity_;
}

glm::vec3 RigidBodyComponent::getAcceleration() {
    return acceleration_;
}

void RigidBodyComponent::reset() {
    velocity_ = glm::vec3(0, 0, 0);
}
