#include "aisystem.h"
#include "Engine/Components/aicomponent.h"

AISystem::AISystem()
    : System(typeid(AIComponent))
{

}

void AISystem::update(float deltaTime) {
    for (auto& [id, gameObject]: m_gameObjects) {
        gameObject->getComponent<AIComponent>()->getBehaviorTree()->update(deltaTime);
    }
}

