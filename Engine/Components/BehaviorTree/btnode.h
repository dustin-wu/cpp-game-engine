#pragma once

enum class Status {RUNNING, SUCCESS, FAILURE};

class BTNode
{
public:
    virtual ~BTNode() = default;
    virtual Status update(float deltaTime) = 0;
    virtual void reset() = 0;
};


