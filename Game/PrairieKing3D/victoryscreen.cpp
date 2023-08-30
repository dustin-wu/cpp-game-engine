#include "victoryscreen.h"
#include "Game/PrairieKing3D/gamescreen.h"
#include "Graphics/global.h"
#include "GLFW/glfw3.h"

namespace PrairieKing3D {

VictoryScreen::VictoryScreen()
{

}

void VictoryScreen::VictoryScreen::render() {
    Global::graphics.setClearColor(glm::vec3(0, 0, 0));
    Global::graphics.clearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Global::graphics.bindShader("text");
    Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "VICTORY", glm::ivec2(20, 420), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 0.5f, 0.5f, glm::vec3(1, 1, 1));
}

void VictoryScreen::update(double deltaTime) {

}

void VictoryScreen::windowResizeEvent(int width, int height) {
    Global::graphics.setWindowSize(glm::ivec2(width, height));
}

void VictoryScreen::keyEvent(int key, int action) {
    if (action == GLFW_PRESS) {
        m_application.lock()->getScreen<GameScreen>("game")->advanceLevel();
        // Press any key to start
        if (m_application.lock()->getScreen<GameScreen>("game")->levelsLeft()) {
            m_application.lock()->getScreen<GameScreen>("game")->startGame();
            m_application.lock()->setVisibleScreens({"game", "overlay"});
            m_application.lock()->setActiveScreens({"game"});
        } else {
            m_application.lock()->getScreen<GameScreen>("game")->resetLevel();
            m_application.lock()->setVisibleScreens({"title", "overlay"});
            m_application.lock()->setActiveScreens({"title"});
        }

        return;
    }
}

void VictoryScreen::mousePosEvent(double xpos, double ypos) {

}

void VictoryScreen::mouseButtonEvent(int button, int action) {

}

void VictoryScreen::scrollEvent(double distance) {

}

}
