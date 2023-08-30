#include <Engine/component.h>

#pragma once

class BulletComponent : public Component
{
public:
    BulletComponent();
    float getSpeed();

private:
    float bulletSpeed_;
};

