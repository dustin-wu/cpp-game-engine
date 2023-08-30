#pragma once

#include "Engine/screen.h"

namespace PrairieKing3D {


class SettingsScreen : public Screen
{
public:
    SettingsScreen();
    void render() override;
    void update(double deltaTime) override;
    void windowResizeEvent(int width, int height) override;
    void keyEvent(int key, int action) override;
    void mousePosEvent(double xpos, double ypos) override;
    void mouseButtonEvent(int button, int action) override;
    void scrollEvent(double distance) override;

};

}
