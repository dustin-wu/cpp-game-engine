#include "Graphics/camera.h"
#include "Graphics/modeltransform.h"
#include "Graphics/shape.h"
#include <Engine/screen.h>

#pragma once

namespace PrairieKing3D {

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
    std::shared_ptr<GameWorld> getGameWorld() {return m_gameWorld;}
    void startGame();
    void advanceLevel(){currentLevel++;}
    void resetLevel(){currentLevel = 1;}
    bool levelsLeft(){return currentLevel <= maxLevel;}

private:
    std::shared_ptr<GameWorld> m_gameWorld;
    int currentLevel = 1;
    int maxLevel = 3;
};

}
