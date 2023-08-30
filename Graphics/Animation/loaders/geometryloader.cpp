#include "geometryloader.h"
#include "tixml2ex.h"
#include <iostream>

GeometryLoader::GeometryLoader(tinyxml2::XMLElement* geometryNode, std::vector<std::shared_ptr<VertexSkinData>> vertexWeights)
    : vertexWeights_(vertexWeights),
      meshData_(geometryNode->FirstChildElement("geometry")->FirstChildElement("mesh"))
{}

std::shared_ptr<MeshData> GeometryLoader::extractModelData() {
    readRawData();
    assembleVertices();
    removeUnusedVertices();
    initArrays();
    convertDataToArrays();
    std::cout << "vertices loaded: " << verticesArray_.size() / 3 << std::endl;
    std::cout << "normals loaded: " << normalsArray_.size() / 3 << std::endl;
    std::cout << "uvs loaded: " << texturesArray_.size() / 2 << std::endl;
    std::cout << "indices loaded: " << indices_.size() << " " << indicesArray_.size() << std::endl;
    std::cout << "joints loaded: " << jointIDsArray_.size() / 3 << std::endl;
    std::cout << "weights loaded: " << weightsArray_.size() / 3 << std::endl;
    return std::make_shared<MeshData>(verticesArray_, texturesArray_, normalsArray_, indicesArray_, jointIDsArray_, weightsArray_);
}

void GeometryLoader::readRawData() {
    readPositions();
    readNormals();
    readTextureCoords();
}

void GeometryLoader::readPositions() {
    std::string positionsID = meshData_->FirstChildElement("vertices")->FirstChildElement("input")->Attribute("source");
    positionsID = positionsID.substr(1);
    tinyxml2::XMLElement* positionsData = tinyxml2::find_element(meshData_, "source[@id='" + positionsID + "']")->FirstChildElement("float_array");
    int count = std::stoi(positionsData->Attribute("count"));
    std::vector<std::string> posData = splitString(positionsData->GetText());
    for (int i = 0; i < count/3; i++) {
        float x = std::stof(posData[i * 3]);
        float y = std::stof(posData[i * 3 + 1]);
        float z = std::stof(posData[i * 3 + 2]);
        glm::vec4 position4 = glm::vec4(x, y, z, 1.f);
        position4 = CORRECTION * position4;
        glm::vec3 position = glm::vec3(position4);
        int vertexIndex = vertices_.size();
        vertices_.push_back(std::make_shared<Vertex>(vertexIndex, position, vertexWeights_[vertexIndex]));
    }
}

void GeometryLoader::readNormals() {
    std::string normalsID = tinyxml2::find_element(meshData_->FirstChildElement("triangles"), "input[@semantic='NORMAL']")->Attribute("source");
    normalsID = normalsID.substr(1);
    tinyxml2::XMLElement* normalsData = tinyxml2::find_element(meshData_, "source[@id='" + normalsID + "']")->FirstChildElement("float_array");
    int count = std::stoi(normalsData->Attribute("count"));
    std::vector<std::string> normData = splitString(normalsData->GetText());
    for (int i = 0; i < count/3; i++) {
        float x = std::stof(normData[i * 3]);
        float y = std::stof(normData[i * 3 + 1]);
        float z = std::stof(normData[i * 3 + 2]);
        glm::vec4 norm4 = glm::vec4(x, y, z, 0.f);
        norm4 = CORRECTION * norm4;
        glm::vec3 norm = glm::vec3(norm4);
        normals_.push_back(norm);
    }
}

void GeometryLoader::readTextureCoords() {
    std::string texCoordsID = tinyxml2::find_element(meshData_->FirstChildElement("triangles"), "input[@semantic='TEXCOORD']")->Attribute("source"); // BLENDER
    texCoordsID = texCoordsID.substr(1);
    tinyxml2::XMLElement* texCoordsData = tinyxml2::find_element(meshData_, "source[@id='" + texCoordsID + "']")->FirstChildElement("float_array");
    int count = std::stoi(texCoordsData->Attribute("count"));
    std::vector<std::string> texData = splitString(texCoordsData->GetText());
    for (int i = 0; i < count/2; i++) {
        float u = std::stof(texData[i * 2]);
        float v = std::stof(texData[i * 2 + 1]);
        textures_.push_back(glm::vec2(u, v));
    }
}

void GeometryLoader::assembleVertices() {
    tinyxml2::XMLElement* triangles = meshData_->FirstChildElement("triangles");
    int typeCount = 0;
    for (tinyxml2::XMLElement* childNode = triangles->FirstChildElement("input"); childNode != nullptr; childNode = childNode->NextSiblingElement("input")) {
        typeCount++;
    }
    std::vector<std::string> indexData = splitString(triangles->FirstChildElement("p")->GetText());
    for (int i = 0; i < indexData.size() / typeCount; i++) {
        int positionIndex = std::stoi(indexData[i * typeCount]);
        int normalIndex = std::stoi(indexData[i * typeCount + 1]);
        int texCoordIndex = std::stoi(indexData[i * typeCount + 2]);
        processVertex(positionIndex, normalIndex, texCoordIndex);
    }
}


void GeometryLoader::processVertex(int posIndex, int normIndex, int texIndex) {
    std::shared_ptr<Vertex> currentVertex = vertices_[posIndex];
    if (!currentVertex->isSet()) {
        currentVertex->normalIndex = normIndex;
        currentVertex->textureIndex = texIndex;
        indices_.push_back(posIndex);
    } else {
        alreadyProcessedVertex(currentVertex, normIndex, texIndex);
    }
}

void GeometryLoader::alreadyProcessedVertex(std::shared_ptr<Vertex> previousVertex, int newNormIndex, int newTexIndex) {
    if (previousVertex->hasSameTextureAndNormal(newNormIndex, newTexIndex)) {
        indices_.push_back(previousVertex->index);
    } else {
        std::shared_ptr<Vertex> newVertex = previousVertex->duplicateVertex;
        if (newVertex != nullptr) {
            alreadyProcessedVertex(newVertex, newNormIndex, newTexIndex);
        } else {
            std::shared_ptr<Vertex> dupeVertex = std::make_shared<Vertex>(vertices_.size(), previousVertex->position, previousVertex->vertexSkinData);
            dupeVertex->normalIndex = newNormIndex;
            dupeVertex->textureIndex = newTexIndex;
            previousVertex->duplicateVertex = dupeVertex;
            vertices_.push_back(dupeVertex);
            indices_.push_back(dupeVertex->index);
        }
    }
}

void GeometryLoader::convertDataToArrays() {
    for (int i = 0; i < vertices_.size(); i++) {
        std::shared_ptr<Vertex> currentVertex = vertices_[i];
        glm::vec3 position = currentVertex->position;
        glm::vec3 normalVector = normals_[currentVertex->normalIndex];
        glm::vec2 textureCoord = textures_[currentVertex->textureIndex];
        verticesArray_[i * 3] = position.x;
        verticesArray_[i * 3 + 1] = position.y;
        verticesArray_[i * 3 + 2] = position.z;
        normalsArray_[i * 3] = normalVector.x;
        normalsArray_[i * 3 + 1] = normalVector.y;
        normalsArray_[i * 3 + 2] = normalVector.z;
        texturesArray_[i * 2] = textureCoord.x;
        texturesArray_[i * 2 + 1] = 1 - textureCoord.y;
        std::shared_ptr<VertexSkinData> weights = currentVertex->vertexSkinData;
        jointIDsArray_[i * 3] = weights->jointIDs[0];
        jointIDsArray_[i * 3 + 1] = weights->jointIDs[1];
        jointIDsArray_[i * 3 + 2] = weights->jointIDs[2];
        weightsArray_[i * 3] = weights->weights[0];
        weightsArray_[i * 3 + 1] = weights->weights[1];
        weightsArray_[i * 3 + 2] = weights->weights[2];
    }

    for (int i = 0; i < indices_.size(); i++) {
        indicesArray_[i] = indices_[i];
    }
}

void GeometryLoader::initArrays() {
    verticesArray_.assign(vertices_.size() * 3, 0);
    texturesArray_.assign(vertices_.size() * 2, 0);
    normalsArray_.assign(vertices_.size() * 3, 0);
    jointIDsArray_.assign(vertices_.size() * 3, 0);
    weightsArray_.assign(vertices_.size() * 3, 0);
    indicesArray_.assign(indices_.size(), 0);
}

void GeometryLoader::removeUnusedVertices() {
    for (std::shared_ptr<Vertex> vertex: vertices_) {
        vertex->averageTangents();
        if (!vertex->isSet()) {
            vertex->normalIndex = 0;
            vertex->textureIndex = 0;
        }
    }
}
