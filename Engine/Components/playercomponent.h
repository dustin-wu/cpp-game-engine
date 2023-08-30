#include <Engine/component.h>

#pragma once

class PlayerComponent : public Component
{
public:
    PlayerComponent();

    float getJumpPower();
    void setJumpPower(float jumpPower);
    float getMovementSpeed();
    void setMovementSpeed(float movementSpeed);
    float getHeadTurn();
    void setHeadTurn(float headTurn);
    float getHeadTilt();
    void setHeadTilt(float headTilt);
    float getVerticalVelocity();
    void setVerticalVelocity(float verticalVelocity);
    float getThirdPersonFactor();
    void setThirdPersonFactor(float thirdPersonFactor);
    void reset() override;
    void setEyeHeight(float eyeHeight);
    float getEyeHeight();
    void setMovingLaterally(bool movingLaterally){movingLaterally_=movingLaterally;}
    bool getMovingLaterally(){return movingLaterally_;}

private:
    float jumpPower_;
    float movementSpeed_;
    float headTurn_;
    float headTilt_;
    float verticalVelocity_;
    float thirdPersonFactor_;
    float eyeHeight_;
    bool movingLaterally_;
    float gravityScale_;
};
