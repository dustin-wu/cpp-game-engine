#include "trianglecollisioncomponent.h"

TriangleCollisionComponent::TriangleCollisionComponent(std::shared_ptr<ModelTransform> model, std::vector<glm::vec3>& triangleVertices)
    : CollisionComponent(typeid(TriangleCollisionComponent))
{
    modelMat_ = model->getModelMatrix();

    for (int i = 0; i < triangleVertices.size(); i += 3) {
        glm::vec3 v0 = triangleVertices[i];
        glm::vec3 v1 = triangleVertices[i+1];
        glm::vec3 v2 = triangleVertices[i+2];
        v0 = glm::vec3(modelMat_ * glm::vec4(v0, 1));
        v1 = glm::vec3(modelMat_ * glm::vec4(v1, 1));
        v2 = glm::vec3(modelMat_ * glm::vec4(v2, 1));
        glm::vec3 normal = glm::cross(v1 - v0, v2 - v0);

        Triangle triangle {v0, v1, v2, normal};
        triangles.push_back(triangle);
    }

}


std::vector<Triangle> TriangleCollisionComponent::getTriangles() {
    return triangles;
}


