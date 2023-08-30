#pragma once

#include "Graphics/Particles/particletexture.h"
#include "glm/glm.hpp"
#include <random>

class ParticleSystem
{
public:
    ParticleSystem(ParticleTexture particleTexture, float particlesRate, float averageSpeed, float gravityFactor, float averageLife, float averageScale);
    void setDirection(glm::vec3 direction, float deviation);
    void randomizeRotation();
    void setSpeedError(float error);
    void setLifeError(float error);
    void setScaleError(float error);
    void generateParticlesOverTime(glm::vec3 center, float deltaTime);
    void generateParticles(glm::vec3 center, int count);

private:
    void emitParticle(glm::vec3 center);
    float generateRandomValue(float average, float error);
    float generateRotation();
    glm::vec3 generateRandomUnitVector();
    glm::vec3 generateRandomUnitVectorWithinCone(glm::vec3 coneDirection, float angle);

    float particlesRate_;
    float averageSpeed_;
    float gravityFactor_;
    float averageLife_;
    float averageScale_;
    float speedError_;
    float lifeError_;
    float scaleError_;
    bool randomRotation_;
    bool directional_;
    glm::vec3 direction_;
    float directionDeviation_;
    ParticleTexture particleTexture_;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;

};


