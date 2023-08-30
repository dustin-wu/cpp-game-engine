#pragma once


#include "Engine/system.h"
#include "glm/glm.hpp"
#include <unordered_set>

struct Edge {
    Edge(){}
    Edge(int v0, int v1, glm::vec3 mid) {
        if (v0 <= v1) {
            v0Index = v0;
            v1Index = v1;
        } else {
            v1Index = v0;
            v0Index = v1;
        }
        midpoint = mid;
    }

    int v0Index;
    int v1Index;
    glm::vec3 midpoint;
};

#define INF std::numeric_limits<float>::max();


template <>
struct std::hash<Edge> {
  std::size_t operator()(const Edge& edge) const {
    return std::hash<int>()(edge.v0Index) ^
           std::hash<int>()(edge.v1Index);
  }
};

class NavMeshSystem: public System
{
public:
    NavMeshSystem();
    void loadNavmesh(GameObjectID navmeshID);
    std::vector<glm::vec3> findPath(GameObjectID movingEntity, GameObjectID targetEntity);
    std::vector<glm::vec3> findPathAwayFrom(GameObjectID movingEntity, GameObjectID targetEntity);

private:
    std::unordered_set<Edge> edges;
    std::unordered_map<Edge, std::unordered_set<int>> edgesToTriangles;
    std::unordered_map<Edge, std::vector<Edge>> adjList;
};

