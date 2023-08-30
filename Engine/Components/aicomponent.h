#pragma once

#include <memory>
#include <Engine/component.h>
#include "Engine/Components/BehaviorTree/btnode.h"

class AIComponent : public Component
{
public:
    AIComponent();

    void setBehaviorTree(std::shared_ptr<BTNode> behaviorTree);
    std::shared_ptr<BTNode> getBehaviorTree();

private:
    std::shared_ptr<BTNode> m_behaviorTree;
};

