#include "uniformgridsystem.h"
#include "Engine/Components/CollisionComponents/cylindercollisioncomponent.h"
#include "Engine/Components/transformcomponent.h"

UniformGridSystem::UniformGridSystem()
    : System(typeid(CylinderCollisionComponent))
{
    for (auto& [id, gameObject]: m_gameObjects) {
        prevPosition[id] = gameObject->getComponent<TransformComponent>()->getGlobalTransform()->getPos();
        entityMoved[id] = true;
    }

    cellSize = 3;
}


void UniformGridSystem::trackMovement() {
    for (auto& [id, gameObject]: m_gameObjects) {
        glm::vec3 curPos = gameObject->getComponent<TransformComponent>()->getGlobalTransform()->getPos();
        if (prevPosition[id] != curPos) {
            entityMoved[id] = true;
            prevPosition[id] = curPos;
        } else {
            entityMoved[id] = false;
        }
    }
}

void UniformGridSystem::updateCells() {
    for (auto& [id, gameObject]: m_gameObjects) {
        if (!entityMoved[id]) continue;

        for (CellIndex cellIndex: occupiedCells[id]) {
            cellMap[cellIndex].erase(id);
        }
        occupiedCells[id].clear();

        glm::vec3 aabbCenter = gameObject->getComponent<TransformComponent>()->getGlobalTransform()->getPos();
        float radius = gameObject->getComponent<CylinderCollisionComponent>()->getRadius();
        float height = gameObject->getComponent<CylinderCollisionComponent>()->getHeight();
        glm::vec3 aabbMin = aabbCenter - glm::vec3(radius, height / 2, radius);
        glm::vec3 aabbMax = aabbCenter + glm::vec3(radius, height / 2, radius);
        int cellMin_x = int(aabbMin.x / cellSize);
        int cellMax_x = int(aabbMax.x / cellSize);
        int cellMin_y = int(aabbMin.y / cellSize);
        int cellMax_y = int(aabbMax.y / cellSize);
        int cellMin_z = int(aabbMin.z / cellSize);
        int cellMax_z = int(aabbMax.z / cellSize);

        for (int x = cellMin_x; x <= cellMax_x; x++) {
            for (int y = cellMin_y; y <= cellMax_y; y++) {
                for (int z = cellMin_z; z <= cellMax_z; z++) {
                    CellIndex cellIndex = std::make_tuple(x, y ,z);
                    cellMap[cellIndex].insert(id);
                    occupiedCells[id].insert(cellIndex);
                }
            }
        }
    }
}

std::unordered_set<GameObjectID> UniformGridSystem::getNeighbors(GameObjectID id) {
    std::unordered_set<GameObjectID> neighbors;

    for (CellIndex cellIndex: occupiedCells[id]) {
        for (GameObjectID otherID: cellMap[cellIndex]) {
            if (otherID != id) {
                neighbors.insert(otherID);
            }
        }
    }

    return neighbors;
}

void UniformGridSystem::removeGameObject(GameObjectID ID) {
    for (CellIndex cellIndex: occupiedCells[ID]) {
        cellMap[cellIndex].erase(ID);
    }
    m_gameObjects.erase(ID);
    prevPosition.erase(ID);
    entityMoved.erase(ID);
    occupiedCells.erase(ID);
}
