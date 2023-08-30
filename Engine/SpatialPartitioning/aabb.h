#pragma once

#include <glm/glm.hpp>
#include <vector>

class AABB
{
public:
    // Default, initalize bounds with numeric limits
    AABB();

    // check if two boxes intersect
    bool boxIntersect(AABB& otherBox);

    void expandBounds(glm::vec3 vertex);

    std::pair<bool, float> rayIntersect(glm::vec3 eyePoint, glm::vec3 rayDirection);

    float getMinX() {return minX;}
    float getMaxX() {return maxX;}
    float getMinY() {return minY;}
    float getMaxY() {return maxY;}
    float getMinZ() {return minZ;}
    float getMaxZ() {return maxZ;}


private:
    float minX, maxX, minY, maxY, minZ, maxZ;
};
