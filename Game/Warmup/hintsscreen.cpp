#include "Graphics/global.h"
#include "hintsscreen.h"

HintsScreen::HintsScreen()
    : titleHint("Press T to go to title")
    , settingsHint("Press R to open settings")
    , startHint("Press any key to start")
    , resumeHint("Press R to resume")
    , controlsHint("WASD to move, hold and drag right mouse to turn, space to jump")
    , exitHint("Press ESC to exit")
{

}

void HintsScreen::HintsScreen::render() {
    std::vector<std::string> hintsToPrint;
    std::vector<std::string> activeScreens = m_application.lock()->getActiveScreens();
    for (std::string& activeScreen: activeScreens) {
        if (activeScreen == "title") {
            hintsToPrint.push_back(startHint);
            hintsToPrint.push_back(exitHint);
        }
        if (activeScreen == "settings") {
            hintsToPrint.push_back(controlsHint);
            hintsToPrint.push_back(resumeHint);
        }
        if (activeScreen == "game") {
            hintsToPrint.push_back(settingsHint);
            hintsToPrint.push_back(titleHint);
        }
    }

    Global::graphics.bindShader("text");
    for (int i = 0; i < hintsToPrint.size(); i++) {
        Global::graphics.drawUIText(Global::graphics.getFont("opensans"), hintsToPrint[i], glm::ivec2(20, 80 - 30 * (hintsToPrint.size() - 1 - i)), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 0.5f, 0.1f, glm::vec3(1, 1, 1));
    }
}

void HintsScreen::update(double deltaTime) {

}

void HintsScreen::windowResizeEvent(int width, int height) {
    Global::graphics.setWindowSize(glm::ivec2(width, height));
}

void HintsScreen::keyEvent(int key, int action) {

}

void HintsScreen::mousePosEvent(double xpos, double ypos) {

}

void HintsScreen::mouseButtonEvent(int button, int action) {

}

void HintsScreen::scrollEvent(double distance) {

}
