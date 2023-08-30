#pragma once


#include "Engine/Components/BehaviorTree/action.h"
#include "Engine/Components/BehaviorTree/btnode.h"
#include "Engine/gameobject.h"
#include "Graphics/Animation/animatedModel/animatedmodel.h"
#include "Game/ExtremeUnaliving/Systems/healthsystem.h"

namespace ExtremeUnaliving {

class ActionHit : public Action
{
public:
    ActionHit(std::shared_ptr<GameObject> entity, std::shared_ptr<GameObject> target, std::shared_ptr<HealthSystem> healthSystem,
              std::shared_ptr<AnimatedModel> animModel, std::shared_ptr<Animation> animation);
      Status act(float deltaTime) override;
      void reset() override {}

private:
    float coolDown;
    float windUp;
    std::shared_ptr<GameObject> m_entity;
    std::shared_ptr<GameObject> m_target;
    std::shared_ptr<HealthSystem> m_healthSystem;
    std::shared_ptr<AnimatedModel> m_animModel;
    std::shared_ptr<Animation> m_animation;
};

}
