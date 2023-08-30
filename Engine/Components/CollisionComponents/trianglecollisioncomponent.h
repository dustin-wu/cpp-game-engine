#pragma once

#include "Engine/Components/collisioncomponent.h"
#include "Graphics/modeltransform.h"
#include <vector>

struct Triangle {
    glm::vec3 v0;
    glm::vec3 v1;
    glm::vec3 v2;
    glm::vec3 normal;
};


class TriangleCollisionComponent : public CollisionComponent
{
public:
    TriangleCollisionComponent(std::shared_ptr<ModelTransform> model, std::vector<glm::vec3>& triangleVertices);
    std::vector<Triangle> getTriangles();

private:
    std::vector<Triangle> triangles;
    glm::mat4 modelMat_;
};

