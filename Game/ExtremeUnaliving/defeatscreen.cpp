#include "Engine/inputtracker.h"
#include "Game/ExtremeUnaliving/defeatscreen.h"
#include "Game/ExtremeUnaliving/gamescreen.h"
#include "Graphics/global.h"
#include "GLFW/glfw3.h"
#include "Engine/windowhandler.h"

namespace ExtremeUnaliving {

DefeatScreen::DefeatScreen()
{
    playButtonText = std::make_shared<TextInfo>("Try Again", "joystix");
    playButton = std::make_shared<GuiButton>("buttonTexture", glm::vec2(0, -0.3), glm::vec2(0.2, 0.075), playButtonText);
    title = std::make_shared<GuiElement>("defeat", glm::vec2(0, 0.3), glm::vec2(0.4, 0.5));

}


void DefeatScreen::DefeatScreen::render() {
    Global::graphics.setClearColor(glm::vec3(0, 0, 0));
    Global::graphics.clearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    Global::graphics.bindShader("gui");
    Global::graphics.drawGUI(title, GUIAnchorPoint::CENTER);
    Global::graphics.drawGUI(playButton, GUIAnchorPoint::CENTER);
}

void DefeatScreen::update(double deltaTime) {
}

void DefeatScreen::windowResizeEvent(int width, int height) {
    Global::graphics.setWindowSize(glm::ivec2(width, height));
}

void DefeatScreen::keyEvent(int key, int action) {

}

void DefeatScreen::mousePosEvent(double xpos, double ypos) {

}

void DefeatScreen::mouseButtonEvent(int button, int action) {
    if (Input::tracker.isMouseButtonDown(GLFW_MOUSE_BUTTON_1) && playButton->isMousedOver()) {
        m_application.lock()->getScreen<GameScreen>("game")->startGame(false);
        m_application.lock()->setVisibleScreens({"game", "overlay"});
        m_application.lock()->setActiveScreens({"game"});
        Input::tracker.setCursor(false);
        glfwSetInputMode(WindowHandling::handler.getWindowPointer(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void DefeatScreen::scrollEvent(double distance) {

}

}
