#include "animator.h"
#include <iostream>

Animator::Animator()
    : animationTime_(0),
      tBeforeFirstKey(0)
{

}

void Animator::doAnimation(std::shared_ptr<Animation> animation) {
//    animationTime_ = 0;
    currentAnimation_ = animation;
}

void Animator::doAnimation(std::shared_ptr<Animation> animation, float animationTime) {
    animationTime_ = animationTime;
    currentAnimation_ = animation;
}


void Animator::update(std::shared_ptr<Joint> rootJoint, float deltaTime) {
    if (currentAnimation_ == nullptr) return;

    animationTime_ += deltaTime;
    while (animationTime_ > currentAnimation_->getLength()) {
        animationTime_ -= currentAnimation_->getLength();
    }

    mat4map currentPose = calculateCurrentAnimationPose();
    applyPoseToJoints(currentPose, rootJoint, glm::mat4(1.f));
}

mat4map Animator::calculateCurrentAnimationPose() {
    // The current pose is determined by interpolating between the two nearest frames
    std::vector<std::shared_ptr<KeyFrame>> keyFrames = getPreviousAndNextFrames();
    float t = calculateT(keyFrames[0], keyFrames[1]);
    return interpolatePoses(keyFrames[0], keyFrames[1], t);
}

std::vector<std::shared_ptr<KeyFrame>> Animator::getPreviousAndNextFrames() {
    std::vector<std::shared_ptr<KeyFrame>> allFrames = currentAnimation_->getKeyFrames();
    std::shared_ptr<KeyFrame> previousFrame = allFrames[0];
    std::shared_ptr<KeyFrame> nextFrame;
    for (int i = 1; i < allFrames.size(); i++) {
        nextFrame = allFrames[i];
        if (nextFrame->getTimeStamp() > animationTime_) {
            break;
        }
        previousFrame = allFrames[i];
    }
    std::vector<std::shared_ptr<KeyFrame>> output;
    output.push_back(previousFrame);
    output.push_back(nextFrame);
    return output;
}

float Animator::calculateT(std::shared_ptr<KeyFrame> previousFrame, std::shared_ptr<KeyFrame> nextFrame) {
    float totalInterval = nextFrame->getTimeStamp() - previousFrame->getTimeStamp();
    float currentTime = animationTime_ - previousFrame->getTimeStamp() + tBeforeFirstKey;
    if (currentTime < 0) currentTime = 0;
    return currentTime / totalInterval;
}

mat4map Animator::interpolatePoses(std::shared_ptr<KeyFrame> previousFrame, std::shared_ptr<KeyFrame> nextFrame, float t) {
    mat4map currentPose;
    for (auto const& [jointName, previousJointTransform]: previousFrame->getPose()) {
        std::shared_ptr<JointTransform> nextJointTransform = nextFrame->getPose()[jointName];
        std::shared_ptr<JointTransform> currentJointTransform = JointTransform::interpolate(previousJointTransform, nextJointTransform, t);
        currentPose[jointName] = currentJointTransform->getLocalTransform();
    }

    return currentPose;
}

void Animator::applyPoseToJoints(mat4map currentPose, std::shared_ptr<Joint> joint, glm::mat4 parentTransform) {
    glm::mat4 currentLocalTransform = currentPose[joint->getName()];
    glm::mat4 currentTransform = parentTransform * currentLocalTransform;

    for (const std::shared_ptr<Joint>& childJoint: joint->getChildren()) {
        applyPoseToJoints(currentPose, childJoint, currentTransform);
    }
     // multiplying by the inverse gives us the model space pose transform
    currentTransform = currentTransform * joint->getInverseBindTransform();
    joint->setAnimatedTransform(currentTransform);
}


