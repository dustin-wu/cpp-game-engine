#include "Graphics/camera.h"
#include "Graphics/modeltransform.h"
#include "Graphics/shape.h"
#include <Engine/screen.h>

#pragma once

class GameScreen : public Screen
{
public:
    GameScreen();
    void render() override;
    void update(double deltaTime) override;
    void windowResizeEvent(int width, int height) override;
    void keyEvent(int key, int action) override;
    void mousePosEvent(double xpos, double ypos) override;
    void mouseButtonEvent(int button, int action) override;
    void scrollEvent(double distance) override;
};

