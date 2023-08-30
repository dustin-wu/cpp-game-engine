#include "animatedmodel.h"
#include "../animation/animator.h"

AnimatedModel::AnimatedModel(std::shared_ptr<MultiBufferVAO> vao, std::string texture, std::shared_ptr<Joint> rootJoint, int jointCount)
    : vao_(vao),
      texture_(texture),
      rootJoint_(rootJoint),
      jointCount_(jointCount)
{
    animator_ = std::make_shared<Animator>();
    rootJoint_->calcInverseBindTransform(glm::mat4(1));
    std::cout << jointCount_ << std::endl;
}

void AnimatedModel::doAnimation(std::shared_ptr<Animation> animation) {
    animator_->doAnimation(animation);
}

void AnimatedModel::doAnimation(std::shared_ptr<Animation> animation, float t) {
    animator_->doAnimation(animation, t);
}


void AnimatedModel::update(float deltaTime) {
    animator_->update(getRootJoint(), deltaTime);
}

std::vector<glm::mat4> AnimatedModel::getJointTransforms() {

    std::vector<glm::mat4> jointTransforms;
    for (int i = 0; i < jointCount_; i++) {
        jointTransforms.push_back(glm::mat4(1));
    }
    fillWithJointTransforms(rootJoint_, jointTransforms);
    return jointTransforms;
}

void AnimatedModel::fillWithJointTransforms(std::shared_ptr<Joint> headJoint, std::vector<glm::mat4>& jointTransforms) {
    jointTransforms[headJoint->getIndex()] = CORRECTION * headJoint->getAnimatedTransform();
    for (const std::shared_ptr<Joint>& child: headJoint->getChildren()) {
        fillWithJointTransforms(child, jointTransforms);
    }
}
