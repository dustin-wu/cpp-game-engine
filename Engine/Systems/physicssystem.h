#pragma once

#include "Engine/system.h"

class PhysicsSystem : public System
{
public:
    PhysicsSystem();

    void applyKinematics(float deltaTimef);


};

