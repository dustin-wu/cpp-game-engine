#pragma once


#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include <memory>

class JointTransform
{
public:
    JointTransform(glm::vec3 position, glm::quat rotation);
    glm::vec3 getPos(){ return position_; }
    glm::quat getRot(){ return rotation_; }
    glm::mat4 getLocalTransform();
    static std::shared_ptr<JointTransform> interpolate(std::shared_ptr<JointTransform> frameA, std::shared_ptr<JointTransform> frameB, float t);

private:
    // Relative to parent
    const glm::vec3 position_;
    const glm::quat rotation_;
};

