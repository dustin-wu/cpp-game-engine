#include "particlesystem.h"
#include "Graphics/Particles/particle.h"
#include "Graphics/global.h"
#include "glm/ext/matrix_transform.hpp"

ParticleSystem::ParticleSystem(ParticleTexture particleTexture, float particlesRate, float averageSpeed, float gravityFactor, float averageLife, float averageScale)
    : particleTexture_(particleTexture),
      particlesRate_(particlesRate),
      averageSpeed_(averageSpeed),
      gravityFactor_(gravityFactor),
      averageLife_(averageLife),
      averageScale_(averageScale),
      directional_(false),
      randomRotation_(false),
      speedError_(0),
      lifeError_(0),
      scaleError_(0),
      gen(rd()), dis(0.0f, 1.0f)
{

}

void ParticleSystem::setDirection(glm::vec3 direction, float deviation) {
    direction_ = direction;
    directionDeviation_ = deviation;
    directional_ = true;
}

void ParticleSystem::randomizeRotation() {
    randomRotation_ = true;
}

void ParticleSystem::setSpeedError(float error) {
    speedError_ = error * averageSpeed_;
}

void ParticleSystem::setLifeError(float error) {
    lifeError_ = error * averageLife_;
}

void ParticleSystem::setScaleError(float error) {
    scaleError_ = error * averageScale_;
}

float ParticleSystem::generateRandomValue(float average, float error) {
    float offset = (dis(gen) - 0.5f) * 2.f * error;
    return average + offset;
}

float ParticleSystem::generateRotation() {
    if (randomRotation_) {
        return dis(gen) * 360.f;
    }
    return 0;
}

glm::vec3 ParticleSystem::generateRandomUnitVector() {
    float theta = dis(gen) * 2.f * M_PI;
    float z = dis(gen) * 2 - 1;
    float sqrtOneMinusZSq = sqrtf(1 - z*z);
    float x = sqrtOneMinusZSq * cosf(theta);
    float y = sqrtOneMinusZSq * sinf(theta);
    return glm::vec3(x, y ,z);
}

glm::vec3 ParticleSystem::generateRandomUnitVectorWithinCone(glm::vec3 coneDirection, float angle) {
    float cosAngle = cosf(angle);
    float theta = dis(gen) * 2.f * M_PI;
    float z = cosAngle + dis(gen) * (1 - cosAngle);
    float sqrtOneMinusZSq = sqrtf(1 - z*z);
    float x = sqrtOneMinusZSq * cosf(theta);
    float y = sqrtOneMinusZSq * sinf(theta);

    glm::vec4 unitDir = glm::vec4(x, y, z, 1);
    if (coneDirection.x != 0 || coneDirection.y != 0 || (coneDirection.z != 1 && coneDirection.z != -1)) {
        glm::vec3 rotAxis = glm::cross(coneDirection, glm::vec3(0, 0, 1));
        rotAxis = glm::normalize(rotAxis);
        float rotAngle = acosf(glm::dot(coneDirection, glm::vec3(0, 0, 1)));
        glm::mat4 rotMatrix = glm::rotate(glm::mat4(1), -rotAngle, rotAxis);
        unitDir = rotMatrix * unitDir;
    }
    else if (coneDirection.z == -1) {
        unitDir.z *= -1;
    }
    return unitDir;
}

void ParticleSystem::emitParticle(glm::vec3 center) {
    glm::vec3 velocity;
    if (directional_) {
        velocity = generateRandomUnitVectorWithinCone(direction_, directionDeviation_);
    } else {
        velocity = generateRandomUnitVector();
    }

    velocity = glm::normalize(velocity);
    velocity *= generateRandomValue(averageSpeed_, speedError_);
    float scale = generateRandomValue(averageScale_, scaleError_);
    float lifeLength = generateRandomValue(averageLife_, lifeError_);
    Particle particle(particleTexture_, center, velocity, gravityFactor_, generateRotation(), scale, lifeLength);
    Global::particleMaster.addParticle(particle);
}

void ParticleSystem::generateParticlesOverTime(glm::vec3 center, float deltaTime) {
    float nParticlesf = particlesRate_ * deltaTime;
    int nParticles = floor(nParticlesf);
    float partial = nParticlesf - nParticles;
    for (int i = 0; i < nParticles; i++) {
        emitParticle(center);
    }
    if (dis(gen) < partial) {
        emitParticle(center);
    }
}

void ParticleSystem::generateParticles(glm::vec3 center, int count) {
    for (int i = 0; i < count; i++) {
        emitParticle(center);
    }
}

