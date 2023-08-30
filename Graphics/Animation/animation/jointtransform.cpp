#include "jointtransform.h"
#include <iostream>

JointTransform::JointTransform(glm::vec3 position, glm::quat rotation)
    : position_(position),
      rotation_(rotation)
{
}

glm::mat4 JointTransform::getLocalTransform() {
    glm::mat4 localMat = glm::mat4(1.f);
    localMat = glm::translate(localMat, position_);
    localMat = localMat * glm::mat4_cast(rotation_);
    return localMat;
}

std::shared_ptr<JointTransform> JointTransform::interpolate(std::shared_ptr<JointTransform> frameA, std::shared_ptr<JointTransform> frameB, float t) {
    glm::vec3 interPos = glm::mix(frameA->getPos(), frameB->getPos(), t);
    glm::quat interRot = glm::slerp(frameA->getRot(), frameB->getRot(), t);
    return std::make_shared<JointTransform>(interPos, interRot);
}
