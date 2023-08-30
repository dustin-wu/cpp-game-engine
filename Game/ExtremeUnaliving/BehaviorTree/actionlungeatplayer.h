#pragma once

#include "Engine/Components/BehaviorTree/action.h"
#include "Engine/gameobject.h"
#include "Graphics/Animation/animatedModel/animatedmodel.h"
namespace ExtremeUnaliving {

class ActionLungeAtPlayer : public Action
{
public:
    ActionLungeAtPlayer(std::shared_ptr<GameObject> entity, std::shared_ptr<GameObject> target,
                        std::shared_ptr<AnimatedModel> animModel, std::shared_ptr<Animation> animation);
    Status act(float deltaTime) override;
    void reset() override {}

private:
    std::shared_ptr<GameObject> m_entity;
    std::shared_ptr<GameObject> m_target;
    float lungeCooldown;
    float lungeDuration;
    const float lungeMaxCooldown = 2.0f;
    const float lungeMaxDuration = 1.f;
    std::shared_ptr<AnimatedModel> m_animModel;
    std::shared_ptr<Animation> m_animation;
};

}
