#include <Engine/component.h>

#pragma once

namespace ExtremeUnaliving {

class BulletComponent : public Component
{
public:
    BulletComponent();
    float getSpeed();

private:
    float bulletSpeed_;
};


}
