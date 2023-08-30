#pragma once

#include "Graphics/Animation/loaders/animationdatastructures.h"
#include "Graphics/Animation/loaders/animationloader.h"
#include "Graphics/Animation/loaders/geometryloader.h"
#include "Graphics/Animation/loaders/skeletonloader.h"
#include "Graphics/Animation/loaders/skinloader.h"
#include "tinyxml2.h"

class ColladaLoader
{
public:
    static std::shared_ptr<AnimatedModelData> loadColladaModel(std::string filepath, int maxWeights) {
        tinyxml2::XMLDocument doc;
        doc.LoadFile(filepath.c_str());
        tinyxml2::XMLElement* root = doc.RootElement();

        SkinLoader skinLoader = SkinLoader(root->FirstChildElement("library_controllers"), maxWeights);
        std::shared_ptr<SkinData> skinData = skinLoader.extractSkinData();

        SkeletonLoader skeletonLoader = SkeletonLoader(root->FirstChildElement("library_visual_scenes"), skinData->jointOrder);
        std::shared_ptr<SkeletonData> skeletonData = skeletonLoader.extractBoneData();

        GeometryLoader geometryLoader = GeometryLoader(root->FirstChildElement("library_geometries"), skinData->verticesSkinData);
        std::shared_ptr<MeshData> meshData = geometryLoader.extractModelData();

        return std::make_shared<AnimatedModelData>(skeletonData, meshData);
    }

    static std::shared_ptr<AnimationData> loadColladaAnimation(std::string filepath) {
        tinyxml2::XMLDocument doc;
        doc.LoadFile(filepath.c_str());
        tinyxml2::XMLElement* root = doc.RootElement();

        tinyxml2::XMLElement* animNode = root->FirstChildElement("library_animations");
        tinyxml2::XMLElement* jointsNode = root->FirstChildElement("library_visual_scenes");
        AnimationLoader animationLoader = AnimationLoader(animNode, jointsNode);
        std::shared_ptr<AnimationData> animData = animationLoader.extractAnimation();
        return animData;
    }


private:
    ColladaLoader(){}
};

