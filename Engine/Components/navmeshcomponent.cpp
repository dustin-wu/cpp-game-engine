#include "navmeshcomponent.h"

NavmeshComponent::NavmeshComponent()
{

}


NavmeshComponent::NavmeshComponent(std::vector<glm::vec3> navmeshVertices, std::vector<glm::ivec3> navmeshFaces) {
    navmeshVertices_ = navmeshVertices;
    navmeshFaces_ = navmeshFaces;
}
