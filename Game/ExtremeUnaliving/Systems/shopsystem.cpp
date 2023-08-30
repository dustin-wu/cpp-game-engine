#include "shopsystem.h"
#include "Engine/Components/drawablecomponent.h"
#include "Engine/Components/transformcomponent.h"
#include "Engine/inputtracker.h"
#include "Engine/windowhandler.h"
#include "GLFW/glfw3.h"
#include "Game/ExtremeUnaliving/Components/shopcomponent.h"

namespace ExtremeUnaliving {

ShopSystem::ShopSystem()
    : System(typeid(ShopComponent))
{

}

bool ShopSystem::checkForShops(float deltaTime) {
    for (auto& [id, gameObject]: m_gameObjects) {
        glm::mat4 curRot = gameObject->getComponent<DrawableComponent>()->getLocalTransform()->getRotation();
        gameObject->getComponent<DrawableComponent>()->getLocalTransform()->setRotation(glm::rotate(curRot, (float)M_PI / 4.f * deltaTime, glm::vec3(0,1,0)));
        glm::vec3 checkPos = gameObject->getComponent<TransformComponent>()->getGlobalTransform()->getPos();
        glm::vec3 playerPos = m_gameWorld.lock()->getGameObject(m_gameWorld.lock()->getPlayerID())->getComponent<TransformComponent>()->getGlobalTransform()->getPos();
        if (glm::distance(playerPos, checkPos) <= 2) {
            return true;
        }
    }
    return false;
}

}
