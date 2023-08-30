#pragma once

#include "Engine/Components/BehaviorTree/btnode.h"
#include <vector>
#include <memory>


class Selector : public BTNode
{
public:
    Selector();
    Status update(float deltaTime) override;
    void reset() override;
    void setChildren(std::vector<std::shared_ptr<BTNode>> children){m_children = children;}

private:
    std::vector<std::shared_ptr<BTNode>> m_children;
    std::shared_ptr<BTNode> m_lastRunning;
};

