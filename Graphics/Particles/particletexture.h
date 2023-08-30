#pragma once

#include <string>

class ParticleTexture
{
public:
    ParticleTexture(std::string texture, int atlasRows);
    std::string getTexture() { return texture_; }
    int getAtlasRows() { return atlasRows_; }

private:
    std::string texture_;
    int atlasRows_;
};

