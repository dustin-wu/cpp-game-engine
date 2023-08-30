#include "spawnercomponent.h"
#include <iostream>

namespace ExtremeUnaliving {


SpawnerComponent::SpawnerComponent(glm::vec3 triggerPosition)
    : triggerPosition_(triggerPosition)
{

}


void SpawnerComponent::insertWave(std::vector<SpawnInfo> wave) {
    waves_.push(wave);
    wavesOriginal_.push(wave);
}

void SpawnerComponent::reset() {
    waves_ = wavesOriginal_;
}


std::vector<SpawnInfo> SpawnerComponent::getNextWave() {
//    std::cout << "summoning wave" << std::endl;
    std::vector<SpawnInfo> output = waves_.front();
    waves_.pop();
    return output;
}

}
