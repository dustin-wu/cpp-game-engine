#pragma once

#include "Engine/system.h"
#include "glm/glm.hpp"

namespace PrairieKing3D {

class GunSystem : public System
{
public:
    GunSystem();
    void updateGunTransform(float deltaTimef);
    void fireBulletOnMouse(float deltaTimef);
    void reset() override;
private:
    glm::mat4 gunMat;
    glm::mat4 defaultGunMat;
    int ammoCount;
    int maxAmmoCount;
    float maxCoolDown;
    float coolDown;
    float maxReloadTime;
    float reloadTime;
    bool gunReload;
    bool manualReload;
};

}
