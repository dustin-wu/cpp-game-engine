#pragma once

#include "Engine/Systems/particlesystem.h"
#include "Engine/system.h"
#include "Graphics/modeltransform.h"
#include "glm/glm.hpp"

namespace ExtremeUnaliving {

class GunSystem : public System
{
public:
    GunSystem();
    void updateGunTransform(float deltaTimef);
    std::shared_ptr<ModelTransform> getGunTransform();
    void fireBulletOnMouse(float deltaTimef);
    void reset() override;
    bool isReloading() {return gunReload;}
    void drawGunfire();
    void drawGunParticles();
    int getAmmoCount() {return ammoCount;}
    void spraynpray();
private:
    glm::mat4 gunMat;
    glm::mat4 defaultGunMat;
    glm::mat4 combinedMat;
    int ammoCount;
    int maxAmmoCount;
    float maxCoolDown;
    float coolDown;
    float maxReloadTime;
    float reloadTime;
    bool gunReload;
    bool manualReload;
    bool setGunTransform;
    float gunRange;
    float gunfireDuration;
    bool sprayPurchased = false;
    glm::vec3 gunfireStart;
    glm::vec3 gunfireEnd;
    std::shared_ptr<ModelTransform> initGunTransform_;
    std::shared_ptr<ParticleSystem> m_particleSystem;
};

}
