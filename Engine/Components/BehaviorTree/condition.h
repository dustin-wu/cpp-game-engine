#pragma once

#include "Engine/Components/BehaviorTree/btnode.h"

class Condition : public BTNode
{
public:
    virtual ~Condition() = default;
    virtual Status check(float deltaTime) = 0;
    Status update(float deltaTime) override {return check(deltaTime);}

};
