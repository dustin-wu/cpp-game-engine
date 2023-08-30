#pragma once

#include "Graphics/Animation/animatedModel/animatedmodel.h"
#include "Graphics/Animation/loaders/animationdatastructures.h"
#include "Graphics/Animation/loaders/colladaloader.h"
#include "Graphics/GLWrappers/multibuffervao.h"
#include "Graphics/global.h"
#include <string>
#include <unordered_map>
#include <memory>

class AnimatedEntityLoader
{
public:
    static std::shared_ptr<AnimatedModel> loadEntity(std::string modelFile, std::string texture) {
        std::shared_ptr<AnimatedModelData> entityData;
        if (dataMap.contains(modelFile)) {
            entityData = dataMap[modelFile];
        } else {
            entityData = ColladaLoader::loadColladaModel(modelFile, 3);
            dataMap[modelFile] = entityData;
        }
        std::shared_ptr<MultiBufferVAO> modelVAO = createVAO(entityData->mesh);
        std::shared_ptr<SkeletonData> skeletonData = entityData->joints;
        std::shared_ptr<Joint> headJoint = createJoints(skeletonData->headJoint);
        return std::make_shared<AnimatedModel>(modelVAO, texture, headJoint, skeletonData->jointCount);
    }

private:
    static std::shared_ptr<Joint> createJoints(std::shared_ptr<JointData> data) {
        std::shared_ptr<Joint> joint = std::make_shared<Joint>(data->index, data->nameID, data->bindLocalTransform);
        for (const std::shared_ptr<JointData> &child: data->children) {
            joint->addChild(createJoints(child));
        }
        return joint;
    }

    static std::shared_ptr<MultiBufferVAO> createVAO(std::shared_ptr<MeshData> data) {
        std::shared_ptr<MultiBufferVAO> vao = std::make_shared<MultiBufferVAO>();
        Global::graphics.tempVertices = data->vertices;
        Global::graphics.tempIndices = data->indices;
        vao->bind();
        vao->addIndexBuffer(data->indices);
        vao->addAttribute(0, data->vertices, 3);
        vao->addAttribute(1, data->normals, 3);
        vao->addAttribute(2, data->textureCoords, 2);
        vao->addIntAttribute(3, data->jointIDs, 3);
        vao->addAttribute(4, data->vertexWeights, 3);
        vao->unbind();
        return vao;
    }

    static std::unordered_map<std::string, std::shared_ptr<AnimatedModelData>> dataMap;


    AnimatedEntityLoader(){}
};

