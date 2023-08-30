#include "glm/glm.hpp"
#include <Engine/component.cpp>

#pragma once

class RigidBodyComponent : public Component
{
public:
    RigidBodyComponent();

    void setVelocity(glm::vec3 velocity);
    void setAcceleration(glm::vec3 acceleration);
    void setStationary(bool stationary);
    void setGravity(float gravity);
    void setAffectedByGravity(bool affectedByGravity);
    bool getStationary();
    bool getAffectedByGravity();
    float getGravity();
    void reset() override;
    glm::vec3 getVelocity();
    glm::vec3 getAcceleration();

private:
    bool stationary_;
    bool affectedByGravity_;
    float gravity_;
    glm::vec3 velocity_;
    glm::vec3 acceleration_;
};

