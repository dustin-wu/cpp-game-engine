#include "skinloader.h"
#include "tixml2ex.h"
#include <sstream>

SkinLoader::SkinLoader(tinyxml2::XMLElement* controllersNode, int maxWeights)
    : maxWeights_(maxWeights),
      skinData_(controllersNode->FirstChildElement("controller")->FirstChildElement("skin"))

{

}

std::shared_ptr<SkinData> SkinLoader::extractSkinData() {
    std::vector<std::string> jointsList = loadJointsList();
    std::vector<float> weights = loadWeights();
    tinyxml2::XMLElement* weightsDataNode = skinData_->FirstChildElement("vertex_weights");
    std::vector<int> effectorJointCounts = getEffectorJointsCounts(weightsDataNode);
    std::vector<std::shared_ptr<VertexSkinData>> vertexWeights = getSkinData(weightsDataNode, effectorJointCounts, weights);
    return std::make_shared<SkinData>(jointsList, vertexWeights);
}

std::vector<std::string> SkinLoader::loadJointsList() {
    tinyxml2::XMLElement* inputNode = skinData_->FirstChildElement("vertex_weights");
    std::string jointDataID = tinyxml2::find_element(inputNode, "input[@semantic='JOINT']")->Attribute("source");
    jointDataID = jointDataID.substr(1);
    tinyxml2::XMLElement* jointsNode = tinyxml2::find_element(skinData_, "source[@id='" + jointDataID + "']")->FirstChildElement("Name_array");
    std::vector<std::string> names = splitString(jointsNode->GetText());
    return names;
}

std::vector<float> SkinLoader::loadWeights() {
    tinyxml2::XMLElement* inputNode = skinData_->FirstChildElement("vertex_weights");
    std::string weightsDataID = tinyxml2::find_element(inputNode, "input[@semantic='WEIGHT']")->Attribute("source");
    weightsDataID = weightsDataID.substr(1);
    tinyxml2::XMLElement* weightsNode = tinyxml2::find_element(skinData_, "source[@id='" + weightsDataID + "']")->FirstChildElement("float_array");
    std::vector<std::string> rawData = splitString(weightsNode->GetText());
    std::vector<float> weights;
    for (const auto& str: rawData) {
      weights.push_back(std::stof(str));
    }
    return weights;
}

std::vector<int> SkinLoader::getEffectorJointsCounts(tinyxml2::XMLElement* weightsDataNode) {
    std::vector<std::string> rawData = splitString(weightsDataNode->FirstChildElement("vcount")->GetText());
    std::vector<int> counts;
    for (const auto& str: rawData) {
      counts.push_back(std::stoi(str));
    }
    return counts;
}

std::vector<std::shared_ptr<VertexSkinData>> SkinLoader::getSkinData(tinyxml2::XMLElement* weightsDataNode, std::vector<int> counts, std::vector<float> weights) {
    std::vector<std::string> rawData = splitString(weightsDataNode->FirstChildElement("v")->GetText());
    std::vector<std::shared_ptr<VertexSkinData>> skinningData;
    int pointer = 0;
    for (int count: counts) {
        std::shared_ptr<VertexSkinData> skinData = std::make_shared<VertexSkinData>();
        for (int i = 0; i < count; i++) {
            int jointID = std::stoi(rawData[pointer++]);
            int weightID = std::stoi(rawData[pointer++]);
            skinData->addJointWeight(jointID, weights[weightID]);
        }
        skinData->limitJointNumber(maxWeights_);
        skinningData.push_back(skinData);
    }
    return skinningData;
}
