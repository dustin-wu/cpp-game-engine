#pragma once


#include "Graphics/Particles/particle.h"
#include "Graphics/camera.h"
#include <unordered_map>
#include <vector>

class ParticleMaster
{
public:
    ParticleMaster();
    void setCamera(std::shared_ptr<Camera> camera) {m_camera = camera;}
    void addParticle(Particle particle) {particles[particle.getParticleTexture().getTexture()].push_back(particle);}
    void update(float deltaTime);
    void renderParticles();


private:
    std::unordered_map<std::string, std::vector<Particle>> particles;
    std::shared_ptr<Camera> m_camera;

};

