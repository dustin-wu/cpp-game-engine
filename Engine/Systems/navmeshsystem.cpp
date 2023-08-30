#include "navmeshsystem.h"
#include "Engine/Components/navmeshcomponent.h"
#include "Engine/Components/transformcomponent.h"
#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"
#include <queue>
#include <vector>

bool operator==(const Edge& a, const Edge& b) {
    return (a.v0Index == b.v0Index) && (a.v1Index == b.v1Index);

}

NavMeshSystem::NavMeshSystem()
    : System(typeid(NavmeshComponent))
{

}

void NavMeshSystem::loadNavmesh(GameObjectID navmeshID) {
    edgesToTriangles.clear();
    adjList.clear();
    edges.clear();

    std::shared_ptr<GameObject> navmesh = m_gameObjects[navmeshID];
    std::vector<glm::ivec3> navmeshTriangles = navmesh->getComponent<NavmeshComponent>()->getFaces();
    std::vector<glm::vec3> navmeshVertices = navmesh->getComponent<NavmeshComponent>()->getVertices();
    for (int triIdx = 0; triIdx < navmeshTriangles.size(); triIdx++) {
        glm::ivec3 triangle = navmeshTriangles[triIdx];
        for (int edgeIdx = 0; edgeIdx < 3; edgeIdx++) {
            int v0 = triangle[edgeIdx];
            int v1 = triangle[(edgeIdx+1)%3];
            Edge edge = Edge(v0, v1, (navmeshVertices[v0] + navmeshVertices[v1]) / 2.f);
            edges.insert(edge);
            edgesToTriangles[edge].insert(triIdx);
        }
    }

    std::unordered_set<Edge> edgesNew;
    std::unordered_map<Edge, std::unordered_set<int>> edgesToTrianglesNew;

    for (Edge edge: edges) {
//        std::cout << edge.v0Index << "," << edge.v1Index << ": " << edgesToTriangles[edge].size() << std::endl;
        if (edgesToTriangles[edge].size() >= 2) {
            edgesNew.insert(edge);
            edgesToTrianglesNew[edge] = edgesToTriangles[edge];
        }
    }

    edges = edgesNew;
    edgesToTriangles = edgesToTrianglesNew;

    for (Edge edgeA: edges) {
        for (Edge edgeB: edges) {
            if (edgeA == edgeB) continue;
            for (int triIdx: edgesToTriangles[edgeA]) {
                if (edgesToTriangles[edgeB].count(triIdx)) {
                    adjList[edgeA].push_back(edgeB);
                }
            }
        }
    }
}

std::vector<glm::vec3> NavMeshSystem::findPath(GameObjectID movingEntity, GameObjectID targetEntity) {
    std::vector<glm::vec3> path; // The output path

    glm::vec3 startPos = m_gameWorld.lock()->getGameObject(movingEntity)->getComponent<TransformComponent>()->getGlobalTransform()->getPos();
    glm::vec3 targetPos = m_gameWorld.lock()->getGameObject(targetEntity)->getComponent<TransformComponent>()->getGlobalTransform()->getPos();

    Edge startNode;
    float minDist = INF;
    for (Edge edge: edges) {
        float distSquared = glm::distance2(startPos, edge.midpoint);
        if (distSquared < minDist) {
            startNode = edge;
            minDist = distSquared;
        }
    }

    Edge endNode;
    float minDist1 = INF;
    for (Edge edge: edges) {
        float distSquared = glm::distance2(targetPos, edge.midpoint);
        if (distSquared < minDist1) {
            endNode = edge;
            minDist1 = distSquared;
        }
    }

    if (startNode == endNode) {
        path.push_back(startPos);
        path.push_back(targetPos);
        return path;
    }

//    std::cout << startPos.x << "," << startPos.y << "," << startPos.z << "-> " << targetPos.x << "," << targetPos.y << "," << targetPos.z << std::endl;

    std::unordered_map<Edge, float> fScores;
    std::unordered_map<Edge, float> gScores;
    for (Edge edge: edges) {
        fScores[edge] = INF;
        gScores[edge] = INF;
    }
    gScores[startNode] = 0;
    fScores[startNode] = glm::distance2(endNode.midpoint, startNode.midpoint);

    auto fScoreCmp = [&fScores](Edge edgeA, Edge edgeB) {
        return fScores[edgeA] > fScores[edgeB];
    };

    std::vector<Edge> edgesVector(edges.begin(), edges.end());
    std::priority_queue<Edge, std::vector<Edge>, decltype(fScoreCmp)> open(fScoreCmp);
    open.push(startNode);
    std::unordered_set<Edge> openSet;
    openSet.insert(startNode);

    std::unordered_map<Edge, Edge> cameFrom;

    while (!open.empty()) {
        Edge currentNode = open.top();
        if (currentNode == endNode) {
            path.push_back(targetPos);
            path.push_back(currentNode.midpoint);
            while (cameFrom.contains(currentNode)) {
                currentNode = cameFrom[currentNode];
                path.push_back(currentNode.midpoint);
            }
            path.push_back(startPos);
            std::reverse(path.begin(), path.end());
            return path;
        }
        open.pop();

        for (Edge neighbor: adjList[currentNode]) {
            float tentativeGScore = gScores[currentNode] + glm::distance2(neighbor.midpoint, currentNode.midpoint);
            if (tentativeGScore < gScores[neighbor]) {
                cameFrom[neighbor] = currentNode;
                gScores[neighbor] = tentativeGScore;
                fScores[neighbor] = tentativeGScore + glm::distance2(neighbor.midpoint, endNode.midpoint);
                if (!openSet.count(neighbor)) {
                    openSet.insert(neighbor);
                    open.push(neighbor);
                }
            }

        }
    }

//    std::cout << "path finding failed" << std::endl;

    return path;
}

std::vector<glm::vec3> NavMeshSystem::findPathAwayFrom(GameObjectID movingEntity, GameObjectID targetEntity) {
    std::vector<glm::vec3> path; // The output path

    glm::vec3 startPos = m_gameWorld.lock()->getGameObject(movingEntity)->getComponent<TransformComponent>()->getGlobalTransform()->getPos();
    glm::vec3 targetPos = m_gameWorld.lock()->getGameObject(targetEntity)->getComponent<TransformComponent>()->getGlobalTransform()->getPos();

    Edge startNode;
    float minDist = INF;
    for (Edge edge: edges) {
        float distSquared = glm::distance2(startPos, edge.midpoint);
        if (distSquared < minDist) {
            startNode = edge;
            minDist = distSquared;
        }
    }

    Edge endNode;
    float minDist1 = INF;
    for (Edge edge: edges) {
        float distSquared = glm::distance2(targetPos, edge.midpoint);
        if (distSquared < minDist1) {
            endNode = edge;
            minDist1 = distSquared;
        }
    }

    if (startNode == endNode) {
        path.push_back(startPos);
        path.push_back(targetPos);
        return path;
    }

//    std::cout << startPos.x << "," << startPos.y << "," << startPos.z << "-> " << targetPos.x << "," << targetPos.y << "," << targetPos.z << std::endl;

    std::unordered_map<Edge, float> fScores;
    std::unordered_map<Edge, float> gScores;
    for (Edge edge: edges) {
        fScores[edge] = INF;
        gScores[edge] = INF;
    }
    gScores[startNode] = 0;
    fScores[startNode] = glm::distance2(endNode.midpoint, startNode.midpoint);

    auto fScoreCmp = [&fScores](Edge edgeA, Edge edgeB) {
        return fScores[edgeA] > fScores[edgeB];
    };

    std::vector<Edge> edgesVector(edges.begin(), edges.end());
    std::priority_queue<Edge, std::vector<Edge>, decltype(fScoreCmp)> open(fScoreCmp);
    open.push(startNode);
    std::unordered_set<Edge> openSet;
    openSet.insert(startNode);

    std::unordered_map<Edge, Edge> cameFrom;

    int iter = 0;
    while (!open.empty()) {
        Edge currentNode = open.top();
        if (iter >= 5) {
            path.push_back(currentNode.midpoint);
            while (cameFrom.contains(currentNode)) {
                currentNode = cameFrom[currentNode];
                path.push_back(currentNode.midpoint);
            }
            path.push_back(startPos);
            std::reverse(path.begin(), path.end());
            return path;
        }
        open.pop();

        for (Edge neighbor: adjList[currentNode]) {
            float tentativeGScore = gScores[currentNode] + glm::distance2(neighbor.midpoint, currentNode.midpoint);
            if (tentativeGScore < gScores[neighbor]) {
                cameFrom[neighbor] = currentNode;
                gScores[neighbor] = tentativeGScore;
                fScores[neighbor] = tentativeGScore - glm::distance2(neighbor.midpoint, endNode.midpoint);
                if (!openSet.count(neighbor)) {
                    openSet.insert(neighbor);
                    open.push(neighbor);
                }
            }

        }

        iter++;
    }

//    std::cout << "path finding failed" << std::endl;

    return path;
}
