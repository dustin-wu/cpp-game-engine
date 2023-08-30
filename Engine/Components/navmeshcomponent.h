#pragma once

#include "glm/glm.hpp"
#include <vector>
#include <Engine/component.h>

class NavmeshComponent : public Component
{
public:
    NavmeshComponent();
    NavmeshComponent(std::vector<glm::vec3> navmeshVertices, std::vector<glm::ivec3> navmeshFaces);
    std::vector<glm::vec3> getVertices(){return navmeshVertices_;};
    std::vector<glm::ivec3> getFaces(){return navmeshFaces_;};
private:
    std::vector<glm::vec3> navmeshVertices_;
    std::vector<glm::ivec3> navmeshFaces_;
};

