#include "Game/PrairieKing3D/gamescreen.h"
#include "Graphics/global.h"
#include "titlescreen.h"
#include "GLFW/glfw3.h"

namespace PrairieKing3D {

TitleScreen::TitleScreen()
{

}

void TitleScreen::TitleScreen::render() {
    Global::graphics.setClearColor(glm::vec3(0, 0, 0));
    Global::graphics.clearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Global::graphics.bindShader("text");
    Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Prairie King 3D: Into the 3rd Dimension", glm::ivec2(20, 420), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 0.5f, 0.5f, glm::vec3(1, 1, 1));
}

void TitleScreen::update(double deltaTime) {

}

void TitleScreen::windowResizeEvent(int width, int height) {
    Global::graphics.setWindowSize(glm::ivec2(width, height));
}

void TitleScreen::keyEvent(int key, int action) {
    if (action == GLFW_PRESS) {
        // Press any key to start
        m_application.lock()->getScreen<GameScreen>("game")->resetLevel();
        m_application.lock()->getScreen<GameScreen>("game")->startGame();
        m_application.lock()->setVisibleScreens({"game", "overlay"});
        m_application.lock()->setActiveScreens({"game"});
        return;
    }
}

void TitleScreen::mousePosEvent(double xpos, double ypos) {

}

void TitleScreen::mouseButtonEvent(int button, int action) {

}

void TitleScreen::scrollEvent(double distance) {

}

}
