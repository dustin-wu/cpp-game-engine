#pragma once

#include "Engine/system.h"

class DrawAnimationSystem : public System
{
public:
    DrawAnimationSystem();

    void draw();
    void update(float deltaTime);
};

