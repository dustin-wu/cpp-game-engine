#pragma once

#include "Engine/Components/collisioncomponent.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

class CylinderCollisionComponent : public CollisionComponent
{
public:
    CylinderCollisionComponent();
    CylinderCollisionComponent(bool physics);
    void setRadius(float radius);
    void setHeight(float height);
    void setLocalPos(glm::vec3 localPos);
    float getRadius();
    float getHeight();
    glm::vec3 getLocalPos();
    bool affectedByPhysics() {return physics_;}
    void makeImmovable();
    bool immovable(){return immovable_;}

private:
    glm::vec3 localPos_;
    float radius_;
    float height_;
    bool physics_;
    bool immovable_ = false;
};
