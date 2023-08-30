#include "playercomponent.h"
#include <algorithm>

PlayerComponent::PlayerComponent()
    : movementSpeed_(7)
    , jumpPower_(5)
    , headTilt_(0)
    , headTurn_(0)
    , thirdPersonFactor_(1)
    , eyeHeight_(1.0)
{

}

float PlayerComponent::getJumpPower() {
    return jumpPower_;
}

void PlayerComponent::setJumpPower(float jumpPower) {
    jumpPower_ = jumpPower;
}

float PlayerComponent::getMovementSpeed() {
    return movementSpeed_;
}

void PlayerComponent::setMovementSpeed(float movementSpeed) {
    movementSpeed_ = movementSpeed;
}

float PlayerComponent::getHeadTurn() {
    return headTurn_;
}

void PlayerComponent::setHeadTurn(float headTurn) {
    headTurn_ = headTurn;
}

float PlayerComponent::getHeadTilt() {
    return headTilt_;
}

void PlayerComponent::setHeadTilt(float headTilt) {
    headTilt_ = headTilt;
}

float PlayerComponent::getVerticalVelocity() {
    return verticalVelocity_;
}

void PlayerComponent::setVerticalVelocity(float verticalVelocity) {
    verticalVelocity_ = verticalVelocity;
}

float PlayerComponent::getThirdPersonFactor() {
    return thirdPersonFactor_;
}

void PlayerComponent::setThirdPersonFactor(float thirdPersonFactor) {
    thirdPersonFactor_ = thirdPersonFactor;
}

void PlayerComponent::reset() {
    movementSpeed_ = 7;
    jumpPower_ = 5;
    headTilt_ = 0;
    headTurn_ = 0;
    thirdPersonFactor_ = 1;
}

void PlayerComponent::setEyeHeight(float eyeHeight) {
    eyeHeight_ = eyeHeight;
}


float PlayerComponent::getEyeHeight() {
    return eyeHeight_;
}

