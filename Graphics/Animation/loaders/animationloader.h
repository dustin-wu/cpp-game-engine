#pragma once

#include "Graphics/Animation/loaders/animationdatastructures.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "tinyxml2.h"

class AnimationLoader
{
public:
    AnimationLoader(tinyxml2::XMLElement* animationData, tinyxml2::XMLElement* jointHierarchy);
    std::shared_ptr<AnimationData> extractAnimation();

private:
    std::vector<float> getKeyTimes();
    std::vector<std::shared_ptr<KeyFrameData>> initKeyFrames(std::vector<float> times);
    void loadJointTransforms(std::vector<std::shared_ptr<KeyFrameData>> keyFrames, tinyxml2::XMLElement* jointData, std::string rootNodeID);
    std::string getDataID(tinyxml2::XMLElement* jointData);
    std::string getJointName(tinyxml2::XMLElement* jointData);
    void processTransforms(std::string jointName, std::vector<std::string> rawData, std::vector<std::shared_ptr<KeyFrameData>> keyFrames, bool isRoot);
    std::string findRootJointName();

    glm::mat4 CORRECTION = glm::rotate(glm::mat4(1), -(float)M_PI/2.f, glm::vec3(1, 0, 0));
    tinyxml2::XMLElement* animationData_;
    tinyxml2::XMLElement* jointHierarchy_;
};

