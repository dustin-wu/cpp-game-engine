#include "bvh.h"
#include <algorithm>

BVH::BVH()
{
    isLeaf_ = false;
}

void BVH::buildBVH(std::vector<TriangleWithID> triangles) {
    // Create a bounding volume that encloses all triangles
    boundingVolume = std::make_shared<AABB>();
    for (TriangleWithID triangle: triangles) {
        boundingVolume->expandBounds(triangle.v0);
        boundingVolume->expandBounds(triangle.v1);
        boundingVolume->expandBounds(triangle.v2);
    }

    // If there aren't that many triangles, set node as leaf
    if (triangles.size() <= MAX_OBJECTS_PER_LEAF) {
//        std::cout << "Leaf node created with " << triangles.size() << " triangles" << std::endl;
        isLeaf_ = true;
        leafTriangles = triangles;
        return;
    }

    // Choose the axis to split on based on which axis has the largest range
    float largestRange = 0;
    int axisToSplit = 0;
    for (int i = 0; i < 3; i++) {
        float axisMin = std::max_element(triangles.begin(), triangles.end(),
            [&](TriangleWithID A, TriangleWithID B) { return A.centroid[i] < B.centroid[i];})->centroid[i];
        float axisMax = std::max_element(triangles.begin(), triangles.end(),
            [&](TriangleWithID A, TriangleWithID B) { return A.centroid[i] < B.centroid[i];})->centroid[i];
        float axisRange = abs(axisMax - axisMin);
        if (largestRange < axisRange) {
            largestRange = axisRange;
            axisToSplit = i;
        }
    }

    // Split by the median
    std::sort(triangles.begin(), triangles.end(),
        [&](TriangleWithID A, TriangleWithID B) { return A.centroid[axisToSplit] < B.centroid[axisToSplit];});
    int median = triangles.size() / 2;
    std::vector<TriangleWithID> leftTriangles(triangles.begin(), triangles.begin() + median);
    std::vector<TriangleWithID> rightTriangles(triangles.begin() + median, triangles.end());

    // Create the children
    leftChild = std::make_shared<BVH>();
    leftChild->buildBVH(leftTriangles);
    rightChild = std::make_shared<BVH>();
    rightChild->buildBVH(rightTriangles);
}

void BVH::traverseBVH(std::function<CollisionData(TriangleWithID)> collisionCheck, std::function<bool(AABB)> boundsCheck, CollisionData& currentClosestCollision) {
    // If we're at a leaf, check collisions with every triangle in the node
    if (isLeaf_) {
        for (TriangleWithID triangle: leafTriangles) {
            CollisionData collisionResult = collisionCheck(triangle);
            if (collisionResult.t < currentClosestCollision.t) {
                currentClosestCollision = collisionResult;
            }
        }
    }

    // Otherwise, descend into children if ellipse intersects with bounding volume
    else {
        if (boundsCheck(*leftChild->boundingVolume)) {
            leftChild->traverseBVH(collisionCheck, boundsCheck, currentClosestCollision);
        }
        if (boundsCheck(*rightChild->boundingVolume)) {
            rightChild->traverseBVH(collisionCheck, boundsCheck, currentClosestCollision);
        }
    }
}

