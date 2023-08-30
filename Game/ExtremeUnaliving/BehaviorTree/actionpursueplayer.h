#pragma once

#include "Engine/Components/BehaviorTree/action.h"
#include "Engine/Systems/navmeshsystem.h"
#include "Engine/gameobject.h"
#include "Graphics/Animation/animatedModel/animatedmodel.h"
#include "glm/glm.hpp"

namespace ExtremeUnaliving {

class ActionPursuePlayer : public Action
{
public:
    ActionPursuePlayer(std::shared_ptr<GameObject> entity, std::shared_ptr<GameObject> target, std::shared_ptr<NavMeshSystem> navSystem, std::shared_ptr<AnimatedModel> animModel, std::shared_ptr<Animation> animation);
    Status act(float deltaTime) override;
    void reset() override {}

private:
    std::shared_ptr<GameObject> m_entity;
    std::shared_ptr<GameObject> m_target;
    std::shared_ptr<NavMeshSystem> m_navSystem;
    std::vector<glm::vec3> chaserPath;
    int pathProgress;
    float findPathCooldown;
    glm::vec3 previousPos;
    const float COOLDOWNMAX = 1;
    std::shared_ptr<AnimatedModel> m_animModel;
    std::shared_ptr<Animation> m_animation;
};

}

