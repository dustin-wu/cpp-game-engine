#include "animationloader.h"
#include "glm/gtc/type_ptr.hpp"
#include "tixml2ex.h"
#include <iostream>

AnimationLoader::AnimationLoader(tinyxml2::XMLElement* animationData, tinyxml2::XMLElement* jointHierarchy)
    : animationData_(animationData->FirstChildElement("animation")), // BLENDER TODO: double-check that picking the first animation child is neccessary (appears to be so looking at collada exports I've done recently)
      jointHierarchy_(jointHierarchy)
{

}

std::shared_ptr<AnimationData> AnimationLoader::extractAnimation() {
    std::string rootNode = findRootJointName();
    std::vector<float> times = getKeyTimes();
    float duration = times[times.size()-1];
    std::vector<std::shared_ptr<KeyFrameData>> keyFrames = initKeyFrames(times);
    for (tinyxml2::XMLElement* animationNode = animationData_->FirstChildElement("animation"); animationNode != nullptr; animationNode = animationNode->NextSiblingElement("animation")) {
       loadJointTransforms(keyFrames, animationNode, rootNode);
    }
    return std::make_shared<AnimationData>(duration, keyFrames);
}

std::vector<float> AnimationLoader::getKeyTimes() {
    tinyxml2::XMLElement* timeData = animationData_->FirstChildElement("animation")->FirstChildElement("source")->FirstChildElement("float_array");
    std::vector<std::string> rawTimes = splitString(timeData->GetText());
    std::vector<float> times;
    for (int i = 0; i < rawTimes.size(); i++) {
        times.push_back(std::stof(rawTimes[i]));
    }
    return times;
}


std::vector<std::shared_ptr<KeyFrameData>> AnimationLoader::initKeyFrames(std::vector<float> times) {
    std::vector<std::shared_ptr<KeyFrameData>> keyFrames;
    for (int i = 0 ; i < times.size(); i++) {
        keyFrames.push_back(std::make_shared<KeyFrameData>(times[i]));
    }
    return keyFrames;
}

void AnimationLoader::loadJointTransforms(std::vector<std::shared_ptr<KeyFrameData>> keyFrames, tinyxml2::XMLElement* jointData, std::string rootNodeID) {
    std::string jointNameID = getJointName(jointData);
//    std::cout << "id: " << jointNameID << std::endl;
    std::string dataID = getDataID(jointData);
    tinyxml2::XMLElement* transformData = tinyxml2::find_element(jointData, "source[@id='" + dataID + "']");
    std::vector<std::string> rawData = splitString(transformData->FirstChildElement("float_array")->GetText());
    processTransforms(jointNameID, rawData, keyFrames, jointNameID == rootNodeID);
}

std::string AnimationLoader::getDataID(tinyxml2::XMLElement* jointData) {
    tinyxml2::XMLElement* node = tinyxml2::find_element(jointData->FirstChildElement("sampler"), "input[@semantic='OUTPUT']");
    std::string dataID = node->Attribute("source");
    return dataID.substr(1);
}

std::string AnimationLoader::getJointName(tinyxml2::XMLElement* jointData) {
    tinyxml2::XMLElement* channelNode = jointData->FirstChildElement("channel");
    std::string data = channelNode->Attribute("target");
    data = data.substr(0, data.find('/'));
    data = data.substr(data.find("_") + 1); // get rid of prefix "Armature_" BLENDER
    return data;
}


void AnimationLoader::processTransforms(std::string jointName, std::vector<std::string> rawData, std::vector<std::shared_ptr<KeyFrameData>> keyFrames, bool isRoot) {
    std::vector<float> buffer (16, 0.f);
    for (int i = 0; i < keyFrames.size(); i++) {
        for (int j = 0; j < 16; j++) {
            buffer[j] = std::stof(rawData[i*16 + j]);
        }
        glm::mat4 transform = glm::transpose(glm::make_mat4(buffer.data()));
        if (isRoot) {
            transform = CORRECTION * transform;
        }
        keyFrames[i]->jointTransforms.push_back(std::make_shared<JointTransformData>(jointName, transform));
    }
}

std::string AnimationLoader::findRootJointName() {
    tinyxml2::XMLElement* skeleton = tinyxml2::find_element(jointHierarchy_->FirstChildElement("visual_scene"), "node[@id='Armature']");
    return skeleton->FirstChildElement("node")->Attribute("id");
}
