#include "joint.h"
#include <iostream>

Joint::Joint(int index, std::string name, glm::mat4 localBindTransform)
    : index_(index),
      name_(name),
      localBindTransform_(localBindTransform)
{
}

void Joint::addChild(std::shared_ptr<Joint> child) {
    children_.push_back(child);
}


glm::mat4 Joint::getAnimatedTransform() {
    return animatedTransform_;
}

void Joint::setAnimatedTransform(glm::mat4 animatedTransform) {
    animatedTransform_ = animatedTransform;
}

glm::mat4 Joint::getInverseBindTransform() {
    return inverseBindTransform_;
}

void Joint::calcInverseBindTransform(glm::mat4 parentBindTransform) {
    glm::mat4 bindTransform = parentBindTransform * localBindTransform_;

    inverseBindTransform_ = glm::inverse(bindTransform);

    for (const std::shared_ptr<Joint>& child: children_) {
        child->calcInverseBindTransform(bindTransform);
    }
}

