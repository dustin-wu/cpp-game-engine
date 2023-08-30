#include "aabb.h"

AABB::AABB() {
    minX = minY = minZ = std::numeric_limits<float>::max();
    maxX = maxY = maxZ = std::numeric_limits<float>::lowest();
}

void AABB::expandBounds(glm::vec3 vertex) {
    minX = fmin(minX, vertex.x);
    maxX = fmax(maxX, vertex.x);
    minY = fmin(minY, vertex.y);
    maxY = fmax(maxY, vertex.y);
    minZ = fmin(minZ, vertex.z);
    maxZ = fmax(maxZ, vertex.z);
}

// adapted from https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
bool AABB::boxIntersect(AABB& otherBox) {
    return (minX <= otherBox.maxX &&
    maxX >= otherBox.minX &&
    minY <= otherBox.maxY &&
    maxY >= otherBox.minY &&
    minZ <= otherBox.maxZ &&
    maxZ >= otherBox.minZ);
}

std::pair<bool, float> AABB::rayIntersect(glm::vec3 eyePoint, glm::vec3 rayDirection) {
    float eps = pow(10, -5);
    glm::vec3 dirFrac = glm::vec3(1.0 / (rayDirection.x + eps), 1.0 / (rayDirection.y + eps), 1.0 / (rayDirection.z + eps));
    glm::vec3 bottomLeft = glm::vec3(minX, minY, minZ);
    glm::vec3 topRight = glm::vec3(maxX, maxY, maxZ);
    float t1 = (bottomLeft.x - eyePoint.x) * dirFrac.x;
    float t2 = (topRight.x - eyePoint.x) * dirFrac.x;
    float t3 = (bottomLeft.y - eyePoint.y) * dirFrac.y;
    float t4 = (topRight.y - eyePoint.y) * dirFrac.y;
    float t5 = (bottomLeft.z - eyePoint.z) * dirFrac.z;
    float t6 = (topRight.z - eyePoint.z) * dirFrac.z;

    float tmin = fmax(fmax(fmin(t1, t2), fmin(t3, t4)), fmin(t5, t6));
    float tmax = fmin(fmin(fmax(t1, t2), fmax(t3, t4)), fmax(t5, t6));

    // intersection is behind eyePoint, not valid
    if (tmax < 0) {
        return std::make_pair(false, tmax);
    }
    // no intersection
    if (tmin > tmax)
    {
        return std::make_pair(false, tmax);
    }
    // found intersection
    return std::make_pair(true, tmin);
}



