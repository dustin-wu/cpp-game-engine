#include "jumppadcomponent.h"

namespace ExtremeUnaliving {

JumpPadComponent::JumpPadComponent()
    : jumpPadForce_(15.f)
{

}


void JumpPadComponent::setJumpPadForce(float jumpPadForce) {
    jumpPadForce_ = jumpPadForce;
}

float JumpPadComponent::getJumpPadForce() {
    return jumpPadForce_;
}



}
