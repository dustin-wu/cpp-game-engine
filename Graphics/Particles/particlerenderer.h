#pragma once

#include <GL/glew.h>
#include <vector>
#include "Graphics/Particles/particle.h"

class ParticleRenderer
{
public:
    ParticleRenderer();
    void renderParticle(Particle particle);
    void initialize();

private:
    GLuint m_vao;
    GLuint m_vbo;
};

