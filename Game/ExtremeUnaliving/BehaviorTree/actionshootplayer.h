#pragma once

#include "Engine/Components/BehaviorTree/action.h"
#include "Engine/gameobject.h"
#include "Game/ExtremeUnaliving/Systems/gameinitializersystem.h"

namespace ExtremeUnaliving {

class ActionShootPlayer : public Action
{
public:
    ActionShootPlayer(std::shared_ptr<GameObject> entity, std::shared_ptr<GameObject> target, std::weak_ptr<GameWorld> gameWorld, std::shared_ptr<AnimatedModel> animModel, std::shared_ptr<Animation> animation);
    Status act(float deltaTime) override;
    void reset() override {}

private:
    void addBullet(std::shared_ptr<ModelTransform> bulletModel, glm::vec3 dir);

    std::shared_ptr<GameObject> m_entity;
    std::shared_ptr<GameObject> m_target;
    std::weak_ptr<GameWorld> m_gameWorld;
    std::shared_ptr<AnimatedModel> m_animModel;
    std::shared_ptr<Animation> m_animation;

    float coolDown;
};


}
