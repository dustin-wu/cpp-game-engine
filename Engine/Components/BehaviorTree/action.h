#pragma once

#include "Engine/Components/BehaviorTree/btnode.h"

class Action : public BTNode
{
public:
    virtual ~Action() = default;
    virtual Status act(float deltaTime) = 0;
    Status update(float deltaTime) override {return act(deltaTime);}
};

