#pragma once

#include "glm/glm.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

struct VertexSkinData {
    std::vector<int> jointIDs;
    std::vector<float> weights;

    void addJointWeight(int jointID, float weight) {
        jointIDs.push_back(jointID);
        weights.push_back(weight);
    }

    void limitJointNumber(int maxJoints) {
        if (jointIDs.size() > maxJoints) {
            sortVectors(weights, jointIDs);
            removeExcessJointIDs(maxJoints);
            normalizeVector(weights);
        } else {
            fillEmptyWeights(maxJoints);
            normalizeVector(weights);
        }
    }

    void fillEmptyWeights(int maxJoints) {
        while (jointIDs.size() < maxJoints) {
            jointIDs.push_back(0);
            weights.push_back(0);
        }
    }

    void removeExcessJointIDs(int maxJoints) {
        while (jointIDs.size() > maxJoints) {
            jointIDs.pop_back();
            weights.pop_back();
        }
    }

    static bool greaterComparator(const std::pair<float, int>& a, const std::pair<float, int>& b) {
        return a.first > b.first;  // Sort in descending order based on the first element of the pair
    }

    void sortVectors(std::vector<float>& A, std::vector<int>& B) {
        std::vector<std::pair<float, int>> AB;

        for (size_t i = 0; i < A.size(); i++) {
            AB.push_back(std::make_pair(A[i], B[i]));
        }

        std::sort(AB.begin(), AB.end(), greaterComparator);

        for (size_t i = 0; i < A.size(); i++) {
            A[i] = AB[i].first;
            B[i] = AB[i].second;
        }
    }

    void normalizeVector(std::vector<float>& vec) {
        float sum = 0.0;

        // Calculate the sum of all elements
        for (const auto& value : vec) {
            sum += value;
        }

        // Divide each element by the sum
        for (auto& value : vec) {
            value /= sum;
        }
    }
};

struct Vertex {
    const int NO_INDEX = -1;
    glm::vec3 position;
    int textureIndex = NO_INDEX;
    int normalIndex = NO_INDEX;
    std::shared_ptr<Vertex> duplicateVertex;
    int index;
    float length;
    std::vector<glm::vec3> tangents;
    glm::vec3 averagedTangent = glm::vec3(0, 0, 0);
    std::shared_ptr<VertexSkinData> vertexSkinData;

    Vertex(int index, glm::vec3 position, std::shared_ptr<VertexSkinData> vertexSkinData)
        : index(index), position(position), vertexSkinData(vertexSkinData){}

    void averageTangents() {
        if (tangents.empty()) return;
        for (glm::vec3 tangent: tangents) averagedTangent += tangent;
        averagedTangent = glm::normalize(averagedTangent);
    }

    bool isSet() {
        return textureIndex != NO_INDEX && normalIndex != NO_INDEX;
    }

    bool hasSameTextureAndNormal(int normalIndexOther, int textureIndexOther) {
        return normalIndexOther == normalIndex && textureIndexOther == textureIndex;
    }
};

struct SkinData {
    std::vector<std::string> jointOrder;
    std::vector<std::shared_ptr<VertexSkinData>> verticesSkinData;
    SkinData(std::vector<std::string> jointOrder_, std::vector<std::shared_ptr<VertexSkinData>> verticesSkinData_) {
        jointOrder = jointOrder_;
        verticesSkinData = verticesSkinData_;
    }
};

struct JointData {
    JointData(int index, const std::string& nameID, const glm::mat4& bindLocalTransform)
            : index(index), nameID(nameID), bindLocalTransform(bindLocalTransform){}
    int index;
    std::string nameID;
    glm::mat4 bindLocalTransform;
    std::vector<std::shared_ptr<JointData>> children;
};

struct SkeletonData {
    SkeletonData(int jointCount, std::shared_ptr<JointData> headJoint)
            : jointCount(jointCount), headJoint(headJoint){}
    int jointCount;
    std::shared_ptr<JointData> headJoint;
};

struct JointTransformData {
    JointTransformData(std::string nameID, glm::mat4 jointLocalTransform)
            : nameID(nameID), jointLocalTransform(jointLocalTransform){}
    std::string nameID;
    glm::mat4 jointLocalTransform;
};

struct KeyFrameData {
    KeyFrameData(float t)
        : t(t){}
    float t;
    std::vector<std::shared_ptr<JointTransformData>> jointTransforms;
};

struct MeshData {
    MeshData(std::vector<float> vertices,
             std::vector<float> textureCoords,
             std::vector<float> normals,
             std::vector<int> indices,
             std::vector<int> jointIDs,
             std::vector<float> vertexWeights)
        : vertices(vertices), textureCoords(textureCoords), normals(normals), indices(indices), jointIDs(jointIDs), vertexWeights(vertexWeights)
    {}
    const int DIMENSIONS = 3;
    std::vector<float> vertices;
    std::vector<float> textureCoords;
    std::vector<float> normals;
    std::vector<int> indices;
    std::vector<int> jointIDs;
    std::vector<float> vertexWeights;
};

struct AnimatedModelData {
    AnimatedModelData(std::shared_ptr<SkeletonData> joints, std::shared_ptr<MeshData> mesh)
        : joints(joints), mesh(mesh){}
    std::shared_ptr<SkeletonData> joints;
    std::shared_ptr<MeshData> mesh;
};

struct AnimationData {
    AnimationData(float lengthSeconds, std::vector<std::shared_ptr<KeyFrameData>> keyFrames)
        : lengthSeconds(lengthSeconds), keyFrames(keyFrames){}
    float lengthSeconds;
    std::vector<std::shared_ptr<KeyFrameData>> keyFrames;
};

auto splitString = [](const std::string& s) -> std::vector<std::string> {
    std::stringstream ss(s);
    std::vector<std::string> result;
    std::string word;
    while (ss >> word) {
        result.push_back(word);
    }
    return result;
};
