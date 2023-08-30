#pragma once


#include "Graphics/Particles/particletexture.h"
#include "glm/glm.hpp"

class Particle
{
public:
    Particle(ParticleTexture particleTexture, glm::vec3 position, glm::vec3 velocity, float gravity, float rotation, float scale, float lifeLength);
    glm::vec3 getPosition() {return position_;}
    glm::vec3 getVelocity() {return velocity_;}
    float getGravity() {return gravity_;}
    float getRotation() {return rotation_;}
    float getScale() {return scale_;}
    float getElapsed() {return elapsed_;}
    float getLifeLength() {return lifeLength_;}
    bool update(float deltaTime);
    ParticleTexture getParticleTexture() {return particleTexture_;}

private:
    glm::vec3 position_;
    glm::vec3 velocity_;
    float gravity_;
    float rotation_;
    float scale_;
    float elapsed_;
    float lifeLength_;
    ParticleTexture particleTexture_;
};

