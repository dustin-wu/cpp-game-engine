#pragma once

#include "Graphics/Animation/loaders/animationdatastructures.h"
#include "tinyxml2.h"
#include <string>
#include <vector>


class SkinLoader
{
public:
    SkinLoader(tinyxml2::XMLElement* controllersNode, int maxWeights);
    std::shared_ptr<SkinData> extractSkinData();

private:
    std::vector<std::string> loadJointsList();
    std::vector<float> loadWeights();
    std::vector<int> getEffectorJointsCounts(tinyxml2::XMLElement* weightsDataNode);
    std::vector<std::shared_ptr<VertexSkinData>> getSkinData(tinyxml2::XMLElement* weightsDataNode, std::vector<int> counts, std::vector<float> weights);

    tinyxml2::XMLElement* skinData_;
    const int maxWeights_;
};

