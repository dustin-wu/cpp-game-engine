#pragma once

#include "Engine/Systems/particlesystem.h"
#include "Engine/system.h"
#include "Graphics/Animation/animation/animation.h"
#include "glm/glm.hpp"

#include <Engine/Systems/charactercontrollersystem.h>

namespace ExtremeUnaliving {

class SpecialCharacterControllerSystem : public CharacterControllerSystem
{
public:
    SpecialCharacterControllerSystem();
    void updateCharacterTransform(double deltaTime) override;
    float getStamina() { return stamina; };
    void checkHitProjectile();
    void setWallCheckID(GameObjectID wallCheckID){ m_wallCheckID = wallCheckID; }
    void checkCameraColliding(double deltaTime, glm::vec3 forwards);

private:
    bool isDashing;
    bool isWallJumping;
    bool isWallSliding;
    bool canJump;
    bool canDash;
    float dashDuration;
    float maxDashDuration;
    float dashSpeed;
    float wallJumpDuration;
    float maxWallJumpDuration;
    float wallJumpSpeed;
    float maxStamina;
    float stamina;
    float staminaRecoveryRate;
    int maxNumWallJumps;
    int numWallJumps;
    glm::vec3 dashVector;
    glm::vec3 wallJumpVector;
    std::shared_ptr<Animation> playerRunning;
    std::shared_ptr<Animation> playerIdle;
    std::shared_ptr<Animation> playerJumping;
    std::shared_ptr<ParticleSystem> m_particleSystem;
    GameObjectID m_wallCheckID;
};

}
