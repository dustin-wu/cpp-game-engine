#include "particlemaster.h"
#include "Graphics/global.h"

ParticleMaster::ParticleMaster()
{

}

void ParticleMaster::update(float deltaTime) {
    for (auto& [texture, particlesList]: particles) {
        for (auto it = particlesList.begin(); it != particlesList.end();) {
            bool stillAlive = it->update(deltaTime);
            if (!stillAlive) {
                it = particlesList.erase(it);
//                std::cout << "particle died" << std::endl;
            } else {
                ++it;
            }
        }
    }

}

void ParticleMaster::renderParticles() {
    Global::graphics.renderParticles(particles, m_camera);
}
