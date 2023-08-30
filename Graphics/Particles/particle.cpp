#include "particle.h"
#include "Graphics/global.h"

Particle::Particle(ParticleTexture particleTexture, glm::vec3 position, glm::vec3 velocity, float gravity, float rotation, float scale, float lifeLength)
    : particleTexture_(particleTexture),
      position_(position),
      velocity_(velocity),
      gravity_(gravity),
      rotation_(rotation),
      scale_(scale),
      lifeLength_(lifeLength),
      elapsed_(0)
{
}


bool Particle::update(float deltaTime) {
    velocity_.y -= 9.8f * gravity_ * deltaTime;
    position_ += velocity_ * deltaTime;
    elapsed_ += deltaTime;
    return elapsed_ < lifeLength_;
}
