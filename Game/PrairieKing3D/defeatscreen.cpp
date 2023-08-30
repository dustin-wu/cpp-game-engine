#include "defeatscreen.h"
#include "Graphics/global.h"
#include "GLFW/glfw3.h"

namespace PrairieKing3D {

DefeatScreen::DefeatScreen()
{

}


void DefeatScreen::DefeatScreen::render() {
    Global::graphics.setClearColor(glm::vec3(0, 0, 0));
    Global::graphics.clearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Global::graphics.bindShader("text");
    Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "DEFEAT", glm::ivec2(20, 420), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 0.5f, 0.5f, glm::vec3(1, 1, 1));
}

void DefeatScreen::update(double deltaTime) {

}

void DefeatScreen::windowResizeEvent(int width, int height) {
    Global::graphics.setWindowSize(glm::ivec2(width, height));
}

void DefeatScreen::keyEvent(int key, int action) {
    if (action == GLFW_PRESS) {
        // Press any key to start
        m_application.lock()->setVisibleScreens({"title", "overlay"});
        m_application.lock()->setActiveScreens({"title"});
        return;
    }
}

void DefeatScreen::mousePosEvent(double xpos, double ypos) {

}

void DefeatScreen::mouseButtonEvent(int button, int action) {

}

void DefeatScreen::scrollEvent(double distance) {

}

}
