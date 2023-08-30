#pragma once

#include "Engine/system.h"
namespace ExtremeUnaliving {

class HealthSystem : public System
{
public:
    HealthSystem();
    void takeDamage(int damage);
    int getHealth() {return health_;}
    int getPlayerHealth();
    void reset() override;
    void update(float deltaTime);
private:
    float coolDown;
    int health_;
};


}
