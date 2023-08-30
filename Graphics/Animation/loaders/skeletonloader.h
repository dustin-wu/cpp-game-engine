#pragma once


#include "Graphics/Animation/loaders/animationdatastructures.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "tinyxml2.h"
#include <string>
#include <vector>

class SkeletonLoader
{
public:
    SkeletonLoader(tinyxml2::XMLElement* visualSceneNode, std::vector<std::string> boneOrder);
    std::shared_ptr<SkeletonData> extractBoneData();
    std::shared_ptr<JointData> loadJointData(tinyxml2::XMLElement* jointNode, bool isRoot);
    std::shared_ptr<JointData> extractMainJointData(tinyxml2::XMLElement* jointNode, bool isRoot);

private:
    tinyxml2::XMLElement* armatureData_;
    std::vector<std::string> boneOrder_;
    int jointCount_;
    glm::mat4 CORRECTION = glm::rotate(glm::mat4(1), -(float)M_PI/2.f, glm::vec3(1, 0, 0));
};

