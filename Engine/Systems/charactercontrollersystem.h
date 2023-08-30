#pragma once

#include "Engine/system.h"
#include "glm/glm.hpp"
#include "glm/fwd.hpp"

class CharacterControllerSystem : public System
{
public:
    CharacterControllerSystem();

    virtual void updateCharacterTransform(double deltaTime);
    void fixMovementIfStationary();
    glm::vec3 getCharacterPos();

protected:
    float gravity;
    glm::vec3 oldPos;
};
