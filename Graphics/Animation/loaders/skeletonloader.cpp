#include "skeletonloader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "glm/gtc/type_ptr.hpp"
#include "tixml2ex.h"

SkeletonLoader::SkeletonLoader(tinyxml2::XMLElement* visualSceneNode, std::vector<std::string> boneOrder)
    : boneOrder_(boneOrder),
      armatureData_(tinyxml2::find_element(visualSceneNode->FirstChildElement("visual_scene"), "node[@id='Armature']")),
      jointCount_(0)
{

}

std::shared_ptr<SkeletonData> SkeletonLoader::extractBoneData() {
    tinyxml2::XMLElement* headNode = armatureData_->FirstChildElement("node");
    std::shared_ptr<JointData> headJoint = loadJointData(headNode, true);
    std::cout << "joints loaded: " << jointCount_ << std::endl;

    return std::make_shared<SkeletonData>(jointCount_, headJoint);
}

std::shared_ptr<JointData> SkeletonLoader::loadJointData(tinyxml2::XMLElement* jointNode, bool isRoot) {
    std::shared_ptr<JointData> joint = extractMainJointData(jointNode, isRoot);
    for (tinyxml2::XMLElement* childNode = jointNode->FirstChildElement("node"); childNode != nullptr; childNode = childNode->NextSiblingElement("node")) {
        joint->children.push_back(loadJointData(childNode, false));
    }
    return joint;
}

std::shared_ptr<JointData> SkeletonLoader::extractMainJointData(tinyxml2::XMLElement* jointNode, bool isRoot) {
    std::string nameID = jointNode->Attribute("id");
    nameID = nameID.substr(nameID.find("_") + 1); // get rid of prefix "Armature_" // BLENDER

    int index = std::distance(boneOrder_.begin(), std::find(boneOrder_.begin(), boneOrder_.end(), nameID));
    std::cout << nameID << " " << index << std::endl;
    std::vector<std::string> matrixData = splitString(jointNode->FirstChildElement("matrix")->GetText());
    std::vector<float> matrixDataf;
    for (const std::string& s: matrixData) {
        matrixDataf.push_back(std::stof(s));
    }
    glm::mat4 matrix = glm::transpose(glm::make_mat4(matrixDataf.data()));

    if (isRoot) {
        matrix = CORRECTION * matrix; // go from blender z up to OpenGL y up
    }
    jointCount_++;
    return std::make_shared<JointData>(index, nameID, matrix);
}

