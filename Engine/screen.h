#pragma once

#include "Engine/application.h"
#include <string>

// Forward declaration so that Screen knows about Application and vice-versa
class Application;

class Screen
{
public:
    virtual ~Screen() = default;
    void setApplication(std::shared_ptr<Application> application);
    virtual void render() = 0;
    virtual void update(double deltaTime) = 0;
    virtual void windowResizeEvent(int width, int height) = 0;
    virtual void keyEvent(int key, int action) = 0;
    virtual void mousePosEvent(double xpos, double ypos) = 0;
    virtual void mouseButtonEvent(int button, int action) = 0;
    virtual void scrollEvent(double distance) = 0;

protected:
    // Using weak_ptr to avoid reference cycles
    std::weak_ptr<Application> m_application;
};

