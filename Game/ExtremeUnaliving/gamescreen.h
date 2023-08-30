#pragma once

#include "Engine/screen.h"

namespace ExtremeUnaliving {

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
    int getFramesPerSec() { return (int) framesPerSec_;};
    void startGame(bool resetCheckpoints = true);

private:
    std::shared_ptr<GameWorld> m_gameWorld;
    float framesPerSec_;
    float refreshFPSCounter = 0.1f;
};



}

