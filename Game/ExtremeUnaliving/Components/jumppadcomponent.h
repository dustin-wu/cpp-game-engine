#include <Engine/component.h>

#pragma once

namespace ExtremeUnaliving {

class JumpPadComponent : public Component
{
public:
    JumpPadComponent();
    void setJumpPadForce(float jumpPadForce);
    float getJumpPadForce();

private:
    float jumpPadForce_;
};



}
