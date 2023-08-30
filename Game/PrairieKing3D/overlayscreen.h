#pragma once

#include "Engine/screen.h"

namespace PrairieKing3D {

class OverlayScreen : public Screen
{
public:
    OverlayScreen();
    void render() override;
    void update(double deltaTime) override;
    void windowResizeEvent(int width, int height) override;
    void keyEvent(int key, int action) override;
    void mousePosEvent(double xpos, double ypos) override;
    void mouseButtonEvent(int button, int action) override;
    void scrollEvent(double distance) override;
    void displayEnemyInfo();
private:
    std::string titleOverlay;
    std::string startOverlay;
    std::string settingsOverlay;
    std::string resumeOverlay;
    std::string controlsOverlay;
    std::string exitOverlay;
    std::string victoryOverlay;
    std::string defeatOverlay;
};

}
