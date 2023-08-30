#pragma once

#include "Engine/Components/collisioncomponent.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

class EllipsoidCollisionComponent : public CollisionComponent
{
public:
    EllipsoidCollisionComponent();
    void setRadii(glm::vec3 radii);
    void setLocalPos(glm::vec3 localPos);
    glm::vec3 getRadii();
    glm::vec3 getLocalPos();
    bool physics = true;

private:
    glm::vec3 localPos_;
    glm::vec3 radii_;
};

