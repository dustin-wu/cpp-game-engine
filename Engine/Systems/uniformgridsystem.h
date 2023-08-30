#pragma once


#include "Engine/system.h"
#include "glm/fwd.hpp"
#include <unordered_set>

using CellIndex = std::tuple<int, int, int>;

template <>
struct std::hash<CellIndex> {
  std::size_t operator()(const CellIndex& index) const {
    return std::hash<int>()(std::get<0>(index)) ^
           std::hash<int>()(std::get<1>(index)) ^
           std::hash<int>()(std::get<2>(index));
  }
};


class UniformGridSystem : public System
{
public:
    UniformGridSystem();
    void trackMovement();
    void updateCells();
    std::unordered_set<GameObjectID> getNeighbors(GameObjectID id);
    void removeGameObject(GameObjectID ID) override;


private:
    std::unordered_map<GameObjectID, glm::vec3> prevPosition;
    std::unordered_map<GameObjectID, bool> entityMoved;
    std::unordered_map<GameObjectID, std::unordered_set<CellIndex>> occupiedCells;
    std::unordered_map<CellIndex, std::unordered_set<GameObjectID>> cellMap;
    float cellSize;
};

