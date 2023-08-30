#pragma once


#include "Graphics/Animation/animatedModel/animatedmodel.h"

typedef std::unordered_map<std::string, glm::mat4> mat4map;


class Animator
{
public:
    Animator();
    void doAnimation(std::shared_ptr<Animation> animation);
    void doAnimation(std::shared_ptr<Animation> animation, float animationTime);
    void update(std::shared_ptr<Joint> rootJoint, float deltaTime);

private:
    mat4map calculateCurrentAnimationPose();
    std::vector<std::shared_ptr<KeyFrame>> getPreviousAndNextFrames();
    float calculateT(std::shared_ptr<KeyFrame> previousFrame, std::shared_ptr<KeyFrame> nextFrame);
    mat4map interpolatePoses(std::shared_ptr<KeyFrame> previousFrame, std::shared_ptr<KeyFrame> nextFrame, float t);
    void applyPoseToJoints(mat4map currentPose, std::shared_ptr<Joint> joint, glm::mat4 parentTransform);


    std::shared_ptr<Animation> currentAnimation_;
    float animationTime_;
    float tBeforeFirstKey;
};

