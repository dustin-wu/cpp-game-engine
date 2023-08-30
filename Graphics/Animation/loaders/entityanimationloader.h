#pragma once

#include "Graphics/Animation/animation/animation.h"
#include "colladaloader.h"
#include <iostream>
#include <string>


class EntityAnimationLoader
{
public:
    static std::shared_ptr<Animation> loadAnimation(std::string colladaFile) {
        std::shared_ptr<AnimationData> animationData = ColladaLoader::loadColladaAnimation(colladaFile);
        std::vector<std::shared_ptr<KeyFrame>> keyFrames;
        for (int i = 0; i < animationData->keyFrames.size(); i++) {
            keyFrames.push_back(createKeyFrame(animationData->keyFrames[i]));
        }
        return std::make_shared<Animation>(animationData->lengthSeconds, keyFrames);
    }

private:
    static std::shared_ptr<KeyFrame> createKeyFrame(std::shared_ptr<KeyFrameData> data) {
        jointmap map;
        for (const std::shared_ptr<JointTransformData> &jointData: data->jointTransforms) {
            std::shared_ptr<JointTransform> jointTransform = createTransform(jointData);
            map[jointData->nameID] = jointTransform;
//            std::cout << jointData->nameID << std::endl; // TODO: there doesn't seem to be a head
        }

        return std::make_shared<KeyFrame>(data->t, map);
    }

    static std::shared_ptr<JointTransform> createTransform(std::shared_ptr<JointTransformData> data) {
        glm::mat4 mat = data->jointLocalTransform;
        glm::vec3 translation = glm::vec3(mat[3]);
        glm::quat rotation = glm::quat_cast(mat);
        return std::make_shared<JointTransform>(translation, rotation);
    }


    EntityAnimationLoader(){}
};

