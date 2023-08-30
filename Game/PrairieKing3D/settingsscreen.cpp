#include "Graphics/global.h"
#include "settingsscreen.h"
#include "GLFW/glfw3.h"

namespace PrairieKing3D {

SettingsScreen::SettingsScreen()
{

}

void SettingsScreen::SettingsScreen::render() {
    Global::graphics.bindShader("text");
    Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Settings", glm::ivec2(20, 420), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 0.5f, 0.5f, glm::vec3(1, 1, 1));

}

void SettingsScreen::update(double deltaTime) {

}

void SettingsScreen::windowResizeEvent(int width, int height) {
    Global::graphics.setWindowSize(glm::ivec2(width, height));
}

void SettingsScreen::keyEvent(int key, int action) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_R) {
            m_application.lock()->setVisibleScreens({"game", "overlay"});
            m_application.lock()->setActiveScreens({"game"});
            return;
        }
    }
}

void SettingsScreen::mousePosEvent(double xpos, double ypos) {

}

void SettingsScreen::mouseButtonEvent(int button, int action) {

}

void SettingsScreen::scrollEvent(double distance) {

}

}
