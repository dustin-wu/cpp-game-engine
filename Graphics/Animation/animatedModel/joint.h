#pragma once

#include "glm/glm.hpp"
#include <string>
#include <vector>
#include <memory>

class Joint
{
public:
    Joint(int index, std::string name, glm::mat4 localBindTransform);

    int getIndex() { return index_; }
    std::string getName() { return name_; }
    std::vector<std::shared_ptr<Joint>> getChildren() { return children_; }
    void addChild(std::shared_ptr<Joint> child);
    glm::mat4 getAnimatedTransform();
    void setAnimatedTransform(glm::mat4 animatedTransform);
    glm::mat4 getInverseBindTransform();
    void calcInverseBindTransform(glm::mat4 parentBindTransform);

private:
    glm::mat4 localBindTransform_; // transform with respect to its parent when model is binded (before any animation)
    glm::mat4 inverseBindTransform_; // inverted model space transform
    glm::mat4 animatedTransform_; // model space transform during animation
    const int index_;
    const std::string name_;
    std::vector<std::shared_ptr<Joint>> children_;

};

