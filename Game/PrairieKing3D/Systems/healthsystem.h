#pragma once

#include "Engine/system.h"
namespace PrairieKing3D {

class HealthSystem : public System
{
public:
    HealthSystem();
    void takeDamage(int damage);
    int getHealth() {return health_;}
    std::string getHealthInfo();
    void reset() override;
private:
    int health_;
};


}
