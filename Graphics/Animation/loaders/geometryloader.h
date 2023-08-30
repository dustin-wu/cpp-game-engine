#pragma once

#include "Graphics/Animation/loaders/animationdatastructures.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "tinyxml2.h"
class GeometryLoader
{
public:
    GeometryLoader(tinyxml2::XMLElement* geometryNode, std::vector<std::shared_ptr<VertexSkinData>> vertexWeights);
    std::shared_ptr<MeshData> extractModelData();

private:
    void readRawData();
    void readPositions();
    void readNormals();
    void readTextureCoords();
    void assembleVertices();
    void processVertex(int posIndex, int normIndex, int texIndex);
    void alreadyProcessedVertex(std::shared_ptr<Vertex> previousVertex, int newNormIndex, int newTexIndex);
    void convertDataToArrays();
    void initArrays();
    void removeUnusedVertices();

    glm::mat4 CORRECTION = glm::rotate(glm::mat4(1), -(float)M_PI/2.f, glm::vec3(1, 0, 0));
    tinyxml2::XMLElement* meshData_;
    std::vector<std::shared_ptr<VertexSkinData>> vertexWeights_;
    std::vector<float> verticesArray_;
    std::vector<float> normalsArray_;
    std::vector<float> texturesArray_;
    std::vector<int> indicesArray_;
    std::vector<int> jointIDsArray_;
    std::vector<float> weightsArray_;

    std::vector<std::shared_ptr<Vertex>> vertices_;
    std::vector<glm::vec2> textures_;
    std::vector<glm::vec3> normals_;
    std::vector<int> indices_;
};

