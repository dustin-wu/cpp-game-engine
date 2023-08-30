#include "collisionsystem.h"
#include "Engine/Components/CollisionComponents/cylindercollisioncomponent.h"
#include "Engine/Components/CollisionComponents/ellipsoidcollisioncomponent.h"
#include "Engine/Components/CollisionComponents/trianglecollisioncomponent.h"
#include "Engine/Components/rigidbodycomponent.h"
#include "Engine/Components/transformcomponent.h"
#include "Engine/SpatialPartitioning/aabb.h"
#include "Engine/SpatialPartitioning/bvh.h"
#include "Engine/Systems/uniformgridsystem.h"
#include <glm/gtx/norm.hpp>
#include <glm/gtx/vector_angle.hpp>

typedef unsigned long long GameObjectID;

const bool OPTIMIZE = true;

bool floatEquality(float a, float b) {
   return (abs(a - b) <= pow(10, -3) * abs(a));
}

CollisionSystem::CollisionSystem()
    : System({typeid(CylinderCollisionComponent), typeid(TriangleCollisionComponent), typeid(EllipsoidCollisionComponent)})
{
}

std::unordered_map<GameObjectID, std::vector<CollisionInfo>>& CollisionSystem::getCollisions() {
    return collisions;
}

std::unordered_map<GameObjectID, std::vector<CollisionData>>& CollisionSystem::getEllipsoidTriangleCollisions() {
    return ellipsoidTriangleCollisions;
}

std::unordered_map<GameObjectID, bool>& CollisionSystem::getCollisionBelowMap() {
    return hasCollisionBelow;
}

bool CollisionSystem::getCollisionWithWall(GameObjectID id, glm::vec3& outNormal) {
    if (hasCollisionWithWall[id]) {
        outNormal = wallCollisionNormal[id];
    }
    return hasCollisionWithWall[id];

}

void CollisionSystem::addGameObject(std::shared_ptr<GameObject> gameObject) {
    bool shouldAdd = false;
    for (std::type_index requiredComponentType: requiredComponentTypes_) {
        if (gameObject->hasComponentOfType(requiredComponentType)) {
            shouldAdd = true;
            break;
        }
    }
    if (shouldAdd) {
        if (gameObject->hasComponentOfType(typeid(RigidBodyComponent)) && !gameObject->getComponent<RigidBodyComponent>()->getStationary()) {
            m_kineticGameObjects[gameObject->ID()] = gameObject;
        }
        else {
            m_stationaryGameObjects[gameObject->ID()] = gameObject;
        }
        m_gameObjects[gameObject->ID()] = gameObject;
//        std::cout << "Recieved GameObject with ID: " << gameObject->ID() << std::endl;
    }
}

void CollisionSystem::removeGameObject(GameObjectID ID) {
    m_kineticGameObjects.erase(ID);
    m_stationaryGameObjects.erase(ID);
    m_gameObjects.erase(ID);
    collisions.erase(ID);

}

void CollisionSystem::clearAllCollisions() {
    for (auto& [id, gameObject]: m_gameObjects) {
        collisions[id].clear();
        hasCollisionBelow[id] = false;
        hasCollisionWithWall[id] = false;
    }
}

void CollisionSystem::updateAllCollisions() {
    clearAllCollisions();
    for (auto& [id, gameObject]: m_kineticGameObjects) {
        // Track collisions for all kinetic objects
        updateObjectsCollisions(id);
    }
}

void CollisionSystem::resolveCollisions() {
    for (auto& [id, collisionInfos]: collisions) {
        for (auto collisionInfo: collisionInfos) {
            if (m_gameObjects[collisionInfo.otherID]->getComponent<CylinderCollisionComponent>()->affectedByPhysics() && m_gameObjects[id]->getComponent<CylinderCollisionComponent>()->affectedByPhysics())
                m_gameObjects[id]->getComponent<TransformComponent>()->getGlobalTransform()->translate(collisionInfo.mtvToMove);
        }
    }
}

void CollisionSystem::updateObjectsCollisions(GameObjectID objectID) {
    if (!m_kineticGameObjects.contains(objectID)) {
        std::cout << "Error: trying to update collisions for stationary object!" << std::endl;
        return;
    }

    if (OPTIMIZE) {
        for (GameObjectID otherID: m_gameWorld.lock()->getSystem<UniformGridSystem>()->getNeighbors(objectID)) {
            if (otherID == objectID) continue;
            CollisionCheckOutput collisionCheck = cylinderCylinderCollision(objectID, otherID);
            if (!collisionCheck.isCollided) continue;
            if (m_kineticGameObjects[objectID]->getComponent<CylinderCollisionComponent>()->immovable()) {
                collisions[objectID].push_back(CollisionInfo{otherID, glm::vec3(0,0,0)});
            } else if (m_kineticGameObjects[otherID]->getComponent<CylinderCollisionComponent>()->immovable()) {
                collisions[objectID].push_back(CollisionInfo{otherID, collisionCheck.mtv});
            } else {
                collisions[objectID].push_back(CollisionInfo{otherID, collisionCheck.mtv * 0.5f}); // for kinetic/kinetic collisions, move kinetic object by half of mtv
            }
            if (collisionCheck.mtv.y > 0.0001f) {
                hasCollisionBelow[objectID] = true; // if MTV is going up, there's a collision below
            }
        }
    }

    else {
        for (auto& [otherID, otherObject]: m_kineticGameObjects) {
            if (otherID == objectID) continue;
            CollisionCheckOutput collisionCheck = cylinderCylinderCollision(objectID, otherID);
            if (!collisionCheck.isCollided) continue;
            collisions[objectID].push_back(CollisionInfo{otherID, collisionCheck.mtv * 0.5f}); // for kinetic/kinetic collisions, move kinetic object by half of mtv
            if (collisionCheck.mtv.y > 0.0001f) {
                hasCollisionBelow[objectID] = true; // if MTV is going up, there's a collision below
            }
        }
    }

}

CollisionCheckOutput CollisionSystem::cylinderCylinderCollision(GameObjectID A, GameObjectID B) {
    std::shared_ptr<CylinderCollisionComponent> objectCollider = m_gameObjects[A]->getComponent<CylinderCollisionComponent>();
    glm::vec3 objectPos = m_gameObjects[A]->getComponent<TransformComponent>()->getGlobalTransform()->getPos();
    glm::vec3 objectColliderPos = objectCollider->getLocalPos() + objectPos;
    float objectColliderRadius = objectCollider->getRadius();
    float objectColliderHeight = objectCollider->getHeight();
    std::shared_ptr<CylinderCollisionComponent> otherCollider = m_gameObjects[B]->getComponent<CylinderCollisionComponent>();;
    glm::vec3 otherPos = m_gameObjects[B]->getComponent<TransformComponent>()->getGlobalTransform()->getPos();
    glm::vec3 otherColliderPos = otherCollider->getLocalPos() + otherPos;
    float otherColliderRadius = otherCollider->getRadius();
    float otherColliderHeight = otherCollider->getHeight();

    CollisionCheckOutput circleCheck = circlesOverlapping(objectColliderRadius, objectColliderPos, otherColliderRadius, otherColliderPos);
    CollisionCheckOutput intervalCheck = intervalsOverlapping(objectColliderHeight, objectColliderPos, otherColliderHeight, otherColliderPos);

    if (!(circleCheck.isCollided && intervalCheck.isCollided)) {
        return CollisionCheckOutput{false, glm::vec3(0,0,0)};
    }
    if (glm::length2(circleCheck.mtv) < glm::length2(intervalCheck.mtv)) {
        return CollisionCheckOutput{true, circleCheck.mtv};
    }
    else {
        return CollisionCheckOutput{true, intervalCheck.mtv};
    }
}

CollisionCheckOutput CollisionSystem::circlesOverlapping(float colliderARadius, glm::vec3 colliderAPos, float colliderBRadius, glm::vec3 colliderBPos) {
   colliderAPos.y = 0;
   colliderBPos.y = 0;

   bool overlap = glm::length2(colliderBPos - colliderAPos) < glm::pow(colliderARadius + colliderBRadius, 2);
   if (!overlap) {
       return CollisionCheckOutput{false, glm::vec3(0,0,0)};
   }

   float len = glm::length(colliderBPos - colliderAPos);
   glm::vec3 mtv = (colliderAPos - colliderBPos) / len * (colliderARadius + colliderBRadius - len);
   return CollisionCheckOutput{true, mtv};
}

CollisionCheckOutput CollisionSystem::intervalsOverlapping(float colliderAHeight, glm::vec3 colliderAPos, float colliderBHeight, glm::vec3 colliderBPos) {
    float colliderAMin = colliderAPos.y - colliderAHeight / 2;
    float colliderAMax = colliderAPos.y + colliderAHeight / 2;
    float colliderBMin = colliderBPos.y - colliderBHeight / 2;
    float colliderBMax = colliderBPos.y + colliderBHeight / 2;

    bool overlap = (colliderBMin < colliderAMax) && (colliderAMin < colliderBMax);
    if (!overlap) {
        return CollisionCheckOutput{false, glm::vec3(0,0,0)};
    }

    float aRight = colliderBMax - colliderAMin;
    float aLeft = colliderAMax - colliderBMin;
    if (aRight < aLeft) {
        return CollisionCheckOutput{true, glm::vec3(0, aRight, 0)};
    }
    else {
        return CollisionCheckOutput{true, glm::vec3(0, -aLeft, 0)};
    }
}

void CollisionSystem::collectTriangles() {
    for (auto& [id, triangleObject]: m_stationaryGameObjects) {
        std::shared_ptr<TriangleCollisionComponent> triangleCollider = triangleObject->getComponent<TriangleCollisionComponent>();
        for (Triangle triangle: triangleCollider->getTriangles()) {
            triangles.push_back(TriangleWithID{triangle.v0, triangle.v1, triangle.v2, triangle.normal, (triangle.v0 + triangle.v1 + triangle.v2)/3.f, id});
        }
    }

    bvh = std::make_shared<BVH>();
    bvh->buildBVH(triangles);
}

void CollisionSystem::updateEllipsoidTriangleCollisions() {

    for (auto& [id, gameObject]: m_kineticGameObjects) {
        glm::vec3 currPos = ellipsoidInitPositions[id];
        glm::vec3 nextPos = ellipsoidFinalPositions[id];


        if (glm::length(currPos - nextPos) > 0.0001f) {
            ellipsoidTriangleCollisions[id].clear();
            mtvSlide(id);
        } else {
            mtvSlidePositions[id] = nextPos;
        }

    }
}

void CollisionSystem::resolveEllipsoidTriangleCollisions() {
    for (auto& [id, gameObject]: m_kineticGameObjects) {
        if (!gameObject->getComponent<EllipsoidCollisionComponent>()->physics) continue;
        gameObject->getComponent<TransformComponent>()->getGlobalTransform()->setPos(mtvSlidePositions[id]);
    }
}

void CollisionSystem::setInitialPositions() {
    for (auto& [id, gameObject]: m_kineticGameObjects) {
        ellipsoidInitPositions[id] = gameObject->getComponent<TransformComponent>()->getGlobalTransform()->getPos();
    }
}

void CollisionSystem::setFinalPositions() {
    for (auto& [id, gameObject]: m_kineticGameObjects) {
        ellipsoidFinalPositions[id] = gameObject->getComponent<TransformComponent>()->getGlobalTransform()->getPos();
    }
}

void CollisionSystem::mtvSlide(GameObjectID ellipsoidID) {
    glm::vec3 currPos = ellipsoidInitPositions[ellipsoidID];
    glm::vec3 nextPos = ellipsoidFinalPositions[ellipsoidID];

    for (int i = 0; i < MAX_TRANSLATIONS; i++) {
         CollisionData collision = getClosestCollision(ellipsoidID, currPos, nextPos);

        if (collision.t == INF) {
            mtvSlidePositions[ellipsoidID] = nextPos;
            return;
        }
//        std::cout << "Normal is: " << collision.normal.x << " " << collision.normal.y << " " << collision.normal.z << std::endl;
        currPos = doNudge(ellipsoidID, currPos, collision);
        glm::vec3 dir = nextPos - currPos;
        glm::vec3 dirCorrected = dir - dot(dir, collision.normal) * collision.normal;
        nextPos = currPos + dirCorrected;

        ellipsoidTriangleCollisions[ellipsoidID].push_back(collision);
        // record if ellipsoid is on the ground
        if (glm::dot(glm::vec3(0, 1, 0), collision.normal) > 0.0001f) {
            hasCollisionBelow[ellipsoidID] = true;
        }
        if (abs(angle(collision.normal, glm::vec3(0, 1, 0)) - M_PI/2) < M_PI/16) {
            hasCollisionWithWall[ellipsoidID] = true;
            wallCollisionNormal[ellipsoidID] = collision.normal;
        }
    }
    mtvSlidePositions[ellipsoidID] = currPos;
}

glm::vec3 CollisionSystem::doNudge(GameObjectID ellipsoidID, glm::vec3 currPos, CollisionData collision) {
    glm::vec3 nudge = collision.normal;
    glm::vec3 posNudged = collision.center + nudge * 0.001f;
    for (int i = 0; i < MAX_NUDGES; i++) {
        CollisionData nudgeCollision = getClosestCollision(ellipsoidID, currPos, posNudged);
        if (nudgeCollision.t == INF) {
            currPos = posNudged;
            break;
        }
        else {
            if (glm::length(nudgeCollision.normal - nudge) < 0.0001f || glm::length(nudgeCollision.normal + nudge) < 0.0001f)
                nudge = -nudgeCollision.normal;
            else
                nudge = nudgeCollision.normal;
            posNudged = nudgeCollision.center + nudge * 0.001f;
        }
    }
    return currPos;
}

CollisionData CollisionSystem::getClosestCollision(GameObjectID ellipsoidID, glm::vec3 initPos, glm::vec3 finalPos) {
    float bestT = INF;
    CollisionData dummyCollision {INF, glm::vec3(0), glm::vec3(0), ellipsoidID};
    CollisionData bestCollision = dummyCollision;

    auto collideWithTriangle = [&](TriangleWithID triangle) -> CollisionData {
        return ellipsoidTriangleCollision(ellipsoidID, initPos, finalPos, triangle);
    };

    AABB ellipsoidAABB = AABB();
    std::shared_ptr<GameObject> ellipsoidObject = m_kineticGameObjects[ellipsoidID];
    std::shared_ptr<EllipsoidCollisionComponent> ellipsoidCollider = ellipsoidObject->getComponent<EllipsoidCollisionComponent>();
    glm::vec3 ellipsoidRadii = ellipsoidCollider->getRadii();

    for (int i = 0; i < 3; i++) {
        glm::vec3 dir = glm::vec3(0, 0, 0);
        dir[i] = 1;
        ellipsoidAABB.expandBounds(initPos + ellipsoidRadii[i] * dir);
        ellipsoidAABB.expandBounds(initPos + -ellipsoidRadii[i] * dir);
        ellipsoidAABB.expandBounds(finalPos + ellipsoidRadii[i] * dir);
        ellipsoidAABB.expandBounds(finalPos + -ellipsoidRadii[i] * dir);
    }

    auto inBounds = [&](AABB bounds) -> bool {
        return ellipsoidAABB.boxIntersect(bounds);
    };

    if (OPTIMIZE) {
        bvh->traverseBVH(collideWithTriangle, inBounds, bestCollision);
        bestT = bestCollision.t;
    }

    else {
        for (TriangleWithID triangle: triangles) {
            CollisionData candidateCollision = ellipsoidTriangleCollision(ellipsoidID, initPos, finalPos, triangle);
            if (candidateCollision.t >= 0 && candidateCollision.t <= 1 && candidateCollision.t < bestT) {
                bestT = candidateCollision.t;
                bestCollision = candidateCollision;
            }
        }
    }

    if (bestT > 1) return dummyCollision;

    return bestCollision;
}

float CollisionSystem::rayTraceTriangles(glm::vec3 rayStart, glm::vec3 rayDirection) {
    CollisionData dummyCollision {INF, glm::vec3(0), glm::vec3(0), 0};
    CollisionData bestCollision = dummyCollision;

    auto collideWithTriangle = [&](TriangleWithID triangle) -> CollisionData {
        float t = rayTriangleCollision(rayStart, rayDirection, triangle.v0, triangle.v1, triangle.v2);
        if (t == INF || t < 0) {
            return CollisionData{INF, glm::vec3(0), glm::vec3(0), 0};
        } else {
            return CollisionData{t, triangle.normal, triangle.centroid, triangle.triangleID};
        }
    };

    auto inBounds = [&](AABB bounds) -> bool {
        return bounds.rayIntersect(rayStart, rayDirection).first;
    };

    bvh->traverseBVH(collideWithTriangle, inBounds, bestCollision);

    return bestCollision.t;
}

CollisionData CollisionSystem::ellipsoidTriangleCollision(GameObjectID ellipsoidID, glm::vec3 initPos, glm::vec3 finalPos, TriangleWithID triangle) {
    std::shared_ptr<GameObject> ellipsoidObject = m_kineticGameObjects[ellipsoidID];
    std::shared_ptr<EllipsoidCollisionComponent> ellipsoidCollider = ellipsoidObject->getComponent<EllipsoidCollisionComponent>();
    glm::vec3 ellipsoidInitPos = initPos + ellipsoidCollider->getLocalPos();
    glm::vec3 ellipsoidFinalPos = finalPos + ellipsoidCollider->getLocalPos();
    glm::vec3 ellipsoidRadii = ellipsoidCollider->getRadii();

    // Convert to Unit Sphere space
    glm::vec3 sphereStart = (ellipsoidInitPos - ellipsoidInitPos) / ellipsoidRadii;
    glm::vec3 sphereEnd = (ellipsoidFinalPos - ellipsoidInitPos) / ellipsoidRadii;

    std::vector<float> candidateTs;
    std::vector<std::vector<glm::vec3>> candidateVertices;

    // Convert to Unit Sphere space
    glm::vec3 vert0 = (triangle.v0 - ellipsoidInitPos) / ellipsoidRadii;
    glm::vec3 vert1 = (triangle.v1 - ellipsoidInitPos) / ellipsoidRadii;
    glm::vec3 vert2 = (triangle.v2 - ellipsoidInitPos) / ellipsoidRadii;

    // Case 1: interior
    float candidate0 = sphereInteriorCollision(sphereStart, sphereEnd, vert0, vert1, vert2);
    candidateTs.push_back(candidate0);
    candidateVertices.push_back({triangle.v0, triangle.v1, triangle.v2});

    if (!(candidate0 >= 0 && candidate0 < INF)) {
        // Case 2: edges
        float candidate1 = sphereEdgeCollision(sphereStart, sphereEnd, vert0, vert1);
        candidateTs.push_back(candidate1);
        candidateVertices.push_back({triangle.v0, triangle.v1});
        float candidate2 = sphereEdgeCollision(sphereStart, sphereEnd, vert1, vert2);
        candidateTs.push_back(candidate2);
        candidateVertices.push_back({triangle.v1, triangle.v2});
        float candidate3 = sphereEdgeCollision(sphereStart, sphereEnd, vert2, vert0);
        candidateTs.push_back(candidate3);
        candidateVertices.push_back({triangle.v2, triangle.v0});

        // Case 3: vertices
        float candidate4 = sphereVertexCollision(sphereStart, sphereEnd, vert0);
        candidateTs.push_back(candidate4);
        candidateVertices.push_back({triangle.v0});
        float candidate5 = sphereVertexCollision(sphereStart, sphereEnd, vert1);
        candidateTs.push_back(candidate5);
        candidateVertices.push_back({triangle.v1});
        float candidate6 = sphereVertexCollision(sphereStart, sphereEnd, vert2);
        candidateTs.push_back(candidate6);
        candidateVertices.push_back({triangle.v2});
    }

    float bestT = INF;
    int bestIndex = 0;
    for (int i = 0; i < candidateTs.size(); i++) {
        if (candidateTs[i] >= 0 && candidateTs[i] < bestT) {
            bestT = candidateTs[i];
            bestIndex = i;
        }
    }

    if (bestT == INF) { // no valid collisions found
        CollisionData dummyCollision{INF, glm::vec3(0), glm::vec3(0), triangle.triangleID, "none"};
        return dummyCollision;
    }

    glm::vec3 collisionCenter = initPos + (ellipsoidFinalPos - ellipsoidInitPos) * bestT;
    glm::vec3 collisionNormal;
    std::vector<glm::vec3> vertices = candidateVertices[bestIndex];
    std::string source;
    if (vertices.size() == 1) {
        collisionNormal = collisionCenter - vertices[0];
        source = "vertex";
    }
    else if (vertices.size() == 2) {
        glm::vec3 edge = vertices[1] - vertices[0];
        glm::vec3 centerToEndpoint = collisionCenter+ellipsoidCollider->getLocalPos() - vertices[0];
        float edgeT = glm::dot(centerToEndpoint, edge) / dot(edge, edge);
        glm::vec3 edgeIntersect = vertices[0] + edgeT * edge;
        collisionNormal = collisionCenter+ellipsoidCollider->getLocalPos() - edgeIntersect;

        source = "edge";
    }
    else if (vertices.size() == 3){
        collisionNormal = glm::cross(vertices[1] - vertices[0], vertices[2] - vertices[0]);
        source = "interior";
    }

    collisionNormal = glm::normalize(collisionNormal);

    CollisionData validCollision{bestT, collisionNormal, collisionCenter, triangle.triangleID, source};
    return validCollision;
}

float CollisionSystem::rayTriangleCollision(glm::vec3 rayStart, glm::vec3 rayDirection, glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2) {
    glm::vec3 normal = glm::normalize(glm::cross(vert1 - vert0, vert2 - vert0));
    float t = glm::dot(vert0 - rayStart, normal) / glm::dot(rayDirection, normal);
    glm::vec3 intersect = rayStart + (t * rayDirection);
    glm::vec3 edge0to1 = vert1 - vert0;
    glm::vec3 edge1to2 = vert2 - vert1;
    glm::vec3 edge2to0 = vert0 - vert2;
    glm::vec3 edge0toIntersect = intersect - vert0;
    glm::vec3 edge1toIntersect = intersect - vert1;
    glm::vec3 edge2toIntersect = intersect - vert2;

    if (glm::dot(glm::cross(edge0to1, edge0toIntersect), normal) > 0
            && glm::dot(glm::cross(edge1to2, edge1toIntersect), normal) > 0
            && glm::dot(glm::cross(edge2to0, edge2toIntersect), normal) > 0) {
        return t;
    }

    return INF;
}

float CollisionSystem::sphereInteriorCollision(glm::vec3 sphereStart, glm::vec3 sphereEnd, glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2) {
    glm::vec3 normal = glm::normalize(glm::cross(vert1 - vert0, vert2 - vert0));
    glm::vec3 rayStart = sphereStart - normal;
    glm::vec3 rayEnd = sphereEnd - normal;
    glm::vec3 rayDirection = rayEnd - rayStart;
    float t = glm::dot(vert0 - rayStart, normal) / glm::dot(rayDirection, normal);
    if (t > 1) return INF;
    glm::vec3 intersect = rayStart + (t * rayDirection);
    glm::vec3 edge0to1 = vert1 - vert0;
    glm::vec3 edge1to2 = vert2 - vert1;
    glm::vec3 edge2to0 = vert0 - vert2;
    glm::vec3 edge0toIntersect = intersect - vert0;
    glm::vec3 edge1toIntersect = intersect - vert1;
    glm::vec3 edge2toIntersect = intersect - vert2;

    if (glm::dot(glm::cross(edge0to1, edge0toIntersect), normal) > 0
            && glm::dot(glm::cross(edge1to2, edge1toIntersect), normal) > 0
            && glm::dot(glm::cross(edge2to0, edge2toIntersect), normal) > 0) {
        return t;
    }

    return INF;
}

float CollisionSystem::sphereEdgeCollision(glm::vec3 sphereStart, glm::vec3 sphereEnd, glm::vec3 edgeStart, glm::vec3 edgeEnd) {
    glm::vec3 crossProd0 = glm::cross(sphereEnd - sphereStart, edgeEnd - edgeStart);
    glm::vec3 crossProd1 = glm::cross(sphereStart - edgeStart, edgeEnd - edgeStart);

    float a = glm::length2(crossProd0);
    float b = 2 * glm::dot(crossProd0, crossProd1);
    float c = glm::length2(crossProd1) - glm::length2(edgeEnd - edgeStart);
    float t = quadratic(a, b, c);
    if (t == INF) return INF;

    glm::vec3 intersect = sphereStart + (sphereEnd - sphereStart) * t;
    float intersectionCheck = glm::dot(intersect - edgeStart, edgeEnd - edgeStart);
    if (0 < intersectionCheck && intersectionCheck < glm::length2(edgeEnd - edgeStart)) {
        return t;
    } else {
        return INF;
    }

}

float CollisionSystem::sphereVertexCollision(glm::vec3 sphereStart, glm::vec3 sphereEnd, glm::vec3 vertex) {
    glm::vec3 start = vertex;
    glm::vec3 end = vertex - (sphereEnd - sphereStart);
    glm::vec3 dir = end - start;
    float a = pow(dir.x, 2) + pow(dir.y, 2) + pow(dir.z, 2);
//    if (a == 0) return INF;
    float b = 2*start.x*dir.x + 2*start.y*dir.y + 2*start.z*dir.z;
    float c = pow(start.x, 2) + pow(start.y, 2) + pow(start.z, 2) - 1;
    float t = quadratic(a, b, c);
    if (t == INF) {
        return INF;
    }
    else {
        return t;
    }
}

float CollisionSystem::discriminant(float a, float b, float c) {
    return pow(b, 2) - 4*a*c;
}

float CollisionSystem::quadratic(float a, float b, float c) {
    float d = discriminant(a, b, c);
    float t;
    if (floatEquality(d, 0.0)) {
        t = -b / (2*a);
    }

    else if (d > 0.0) {
        float t1 = (-b - sqrt(d)) / (2*a);
        float t2 = (-b + sqrt(d)) / (2*a);
        if (t1 < 0) t = t2;
        else if (t2 < 0) t = t1;
        else t = fmin(t1, t2);
    }

    else {
        t = INF;
    }

    return t;
}

float CollisionSystem::checkRayHit(GameObjectID id, glm::vec3 rayStart, glm::vec3 rayDirection) {
    std::shared_ptr<CylinderCollisionComponent> objectCollider = m_gameObjects[id]->getComponent<CylinderCollisionComponent>();
    glm::vec3 objectPos = m_gameObjects[id]->getComponent<TransformComponent>()->getGlobalTransform()->getPos();
    glm::vec3 objectColliderPos = objectCollider->getLocalPos() + objectPos;
    glm::vec3 translated = rayStart - objectColliderPos;

    float a = pow(rayDirection.x, 2) + pow(rayDirection.z, 2);
    float b = 2 * (rayDirection.x * translated.x + rayDirection.z * translated.z);
    float c = pow(translated.x, 2) + pow(translated.z, 2) - pow(objectCollider->getRadius(), 2);
    float t = quadratic(a, b, c);

    glm::vec3 intersection = rayStart + t * rayDirection;
    if (intersection.y < objectColliderPos.y - objectCollider->getHeight()/2 || intersection.y > objectColliderPos.y + objectCollider->getHeight()/2) {
        return INF;
    } else {
        return t;
    }
}

