#pragma once

#include "Engine/screen.h"

class HintsScreen : public Screen
{
public:
    HintsScreen();
    void render() override;
    void update(double deltaTime) override;
    void windowResizeEvent(int width, int height) override;
    void keyEvent(int key, int action) override;
    void mousePosEvent(double xpos, double ypos) override;
    void mouseButtonEvent(int button, int action) override;
    void scrollEvent(double distance) override;
private:
    std::string titleHint;
    std::string startHint;
    std::string settingsHint;
    std::string resumeHint;
    std::string controlsHint;
    std::string exitHint;
};

