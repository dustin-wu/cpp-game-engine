#pragma once

#include "Engine/SpatialPartitioning//aabb.h"
#include "Engine/Systems/collisionsystem.h"

class BVH
{
public:
    BVH();

    void buildBVH(std::vector<TriangleWithID> triangles);
    void traverseBVH(std::function<CollisionData(TriangleWithID)> collisionCheck, std::function<bool(AABB)> boundsCheck, CollisionData& currentClosestCollision);

private:
    std::shared_ptr<AABB> boundingVolume;
    std::shared_ptr<BVH> leftChild;
    std::shared_ptr<BVH> rightChild;
    std::vector<TriangleWithID> leafTriangles;
    bool isLeaf_;

    const int MAX_OBJECTS_PER_LEAF = 5;

};
