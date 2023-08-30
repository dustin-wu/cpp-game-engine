#pragma once


#include "Graphics/Animation/animatedModel/joint.h"
#include "Graphics/Animation/animation/animation.h"
#include "Graphics/GLWrappers/multibuffervao.h"
#include "Graphics/GLWrappers/vao.h"
#include <string>
#include <iostream>

class Animator;

class AnimatedModel : public std::enable_shared_from_this<AnimatedModel>
{
public:
    AnimatedModel(std::shared_ptr<MultiBufferVAO> vao, std::string texture, std::shared_ptr<Joint> rootJoint, int jointCount);
    std::shared_ptr<MultiBufferVAO> getVAO() {return vao_;}
    std::string getTexture() { return texture_; }
    std::shared_ptr<Joint> getRootJoint() {return rootJoint_; }
    void doAnimation(std::shared_ptr<Animation> animation);
    void doAnimation(std::shared_ptr<Animation> animation, float t);
    void update(float deltaTime);
    std::vector<glm::mat4> getJointTransforms();

private:
    void fillWithJointTransforms(std::shared_ptr<Joint> headJoint, std::vector<glm::mat4>& jointTransforms);
    glm::mat4 CORRECTION = glm::rotate(glm::mat4(1), -(float)M_PI/2.f, glm::vec3(1, 0, 0));

    std::shared_ptr<MultiBufferVAO> vao_;
    std::string texture_;
    std::shared_ptr<Joint> rootJoint_;
    const int jointCount_;
    std::shared_ptr<Animator> animator_;




};

