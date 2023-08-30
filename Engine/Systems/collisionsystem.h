#pragma once

#include "Engine/Components/collisioncomponent.h"
#include "Engine/gameobject.h"
#include <memory>
#include <Engine/system.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"

struct CollisionInfo {
    GameObjectID otherID;
    glm::vec3 mtvToMove;
};

struct CollisionData {
    float t;
    glm::vec3 normal;
    glm::vec3 center;
    GameObjectID otherID;
    std::string source;
};

struct CollisionCheckOutput {
    bool isCollided;
    glm::vec3 mtv;
};

struct TriangleWithID {
    glm::vec3 v0;
    glm::vec3 v1;
    glm::vec3 v2;
    glm::vec3 normal;
    glm::vec3 centroid;
    GameObjectID triangleID;
};

class BVH;

class CollisionSystem : public System
{
public:
    CollisionSystem();
    void updateAllCollisions();
    void updateEllipsoidTriangleCollisions();
    void resolveCollisions();
    void resolveEllipsoidTriangleCollisions();
    void setInitialPositions();
    void setFinalPositions();
    std::unordered_map<GameObjectID, std::vector<CollisionInfo>>& getCollisions();
    std::unordered_map<GameObjectID, bool>& getCollisionBelowMap();
    std::unordered_map<GameObjectID, std::vector<CollisionData>>& getEllipsoidTriangleCollisions();
    bool getCollisionWithWall(GameObjectID id, glm::vec3& outNormal);
    void addGameObject(std::shared_ptr<GameObject> gameObject) override;
    void removeGameObject(GameObjectID ID) override;
    void clearAllCollisions();
    void collectTriangles();
    float rayTraceTriangles(glm::vec3 rayStart, glm::vec3 rayDirection);
    float checkRayHit(GameObjectID id, glm::vec3 rayStart, glm::vec3 rayDirection);

private:
    void updateObjectsCollisions(GameObjectID objectID);
    CollisionCheckOutput cylinderCylinderCollision(GameObjectID A, GameObjectID B);
    CollisionCheckOutput circlesOverlapping(float colliderARadius, glm::vec3 colliderAPos, float colliderBRadius, glm::vec3 colliderBPos);
    CollisionCheckOutput intervalsOverlapping(float colliderAHeight, glm::vec3 colliderAPos, float colliderBHeight, glm::vec3 colliderBPos);

    void mtvSlide(GameObjectID ellipsoidID);
    glm::vec3 doNudge(GameObjectID ellipsoidID, glm::vec3 currPos, CollisionData collision);
    CollisionData getClosestCollision(GameObjectID ellipsoidID, glm::vec3 initPos, glm::vec3 finalPos);
    CollisionData ellipsoidTriangleCollision(GameObjectID ellipsoidID, glm::vec3 initPos, glm::vec3 finalPos, TriangleWithID triangle);
    float sphereInteriorCollision(glm::vec3 sphereStart, glm::vec3 sphereEnd, glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2);
    float sphereEdgeCollision(glm::vec3 sphereStart, glm::vec3 sphereEnd, glm::vec3 edgeStart, glm::vec3 edgeEnd);
    float sphereVertexCollision(glm::vec3 sphereStart, glm::vec3 sphereEnd, glm::vec3 vertex);
    float discriminant(float a, float b, float c);
    float quadratic(float a, float b, float c);
    float rayTriangleCollision(glm::vec3 rayStart, glm::vec3 rayDirection, glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2);



    std::unordered_map<GameObjectID, std::vector<CollisionInfo>> collisions;
    std::unordered_map<GameObjectID, std::shared_ptr<GameObject>> m_stationaryGameObjects;
    std::unordered_map<GameObjectID, std::shared_ptr<GameObject>> m_kineticGameObjects;
    std::unordered_map<GameObjectID, bool> hasCollisionBelow;
    std::unordered_map<GameObjectID, bool> hasCollisionWithWall;
    std::unordered_map<GameObjectID, glm::vec3> wallCollisionNormal;

    std::unordered_map<GameObjectID, glm::vec3> ellipsoidInitPositions;
    std::unordered_map<GameObjectID, glm::vec3> ellipsoidFinalPositions;
    std::unordered_map<GameObjectID, std::vector<CollisionData>> ellipsoidTriangleCollisions;
    std::unordered_map<GameObjectID, glm::vec3> mtvSlidePositions;

    std::vector<TriangleWithID> triangles;

    const int MAX_NUDGES = 5;
    const int MAX_TRANSLATIONS = 5;
    const float INF = std::numeric_limits<float>::max();

    std::shared_ptr<BVH> bvh;
};

