#include "Engine/inputtracker.h"
#include "Game/ExtremeUnaliving/gamescreen.h"
#include "Graphics/global.h"
#include "settingsscreen.h"
#include "GLFW/glfw3.h"
#include "Engine/windowhandler.h"


namespace ExtremeUnaliving {

SettingsScreen::SettingsScreen()
{
    playButtonText = std::make_shared<TextInfo>("Resume", "joystix");
    playButton = std::make_shared<GuiButton>("buttonTexture", glm::vec2(0, -0.3), glm::vec2(0.15, 0.075), playButtonText);

    exitButtonText = std::make_shared<TextInfo>("Title", "joystix");
    exitButton = std::make_shared<GuiButton>("buttonTexture", glm::vec2(0, -0.5), glm::vec2(0.15, 0.075), exitButtonText);

    tutorial = std::make_shared<GuiElement>("tutorial", glm::vec2(0, 0.2), glm::vec2(0.3, 0.3));

}

void SettingsScreen::SettingsScreen::render() {
    Global::graphics.setClearColor(glm::vec3(0, 0, 0));
//    Global::graphics.clearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    Global::graphics.bindShader("text");
//    Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Settings", glm::ivec2(20, 420), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 1.5f, 0.5f, glm::vec3(1, 1, 1));

    Global::graphics.bindShader("gui");
    background = std::make_shared<GuiElement>("barbackgroundTexture", glm::vec2(0, 0), glm::vec2(0.7, 0.7));
    Global::graphics.drawGUI(background, GUIAnchorPoint::CENTER);

    Global::graphics.drawGUI(playButton, GUIAnchorPoint::CENTER);
    Global::graphics.drawGUI(exitButton, GUIAnchorPoint::CENTER);
    Global::graphics.drawGUI(tutorial, GUIAnchorPoint::CENTER);
}

void SettingsScreen::update(double deltaTime) {

}

void SettingsScreen::windowResizeEvent(int width, int height) {
    Global::graphics.setWindowSize(glm::ivec2(width, height));
}

void SettingsScreen::keyEvent(int key, int action) {
//    if (action == GLFW_PRESS) {
//        // Press any key to start
////        m_application.lock()->getScreen<GameScreen>("game")->resetLevel();
////        m_application.lock()->getScreen<GameScreen>("game")->startGame();
//        m_application.lock()->setVisibleScreens({"game", "overlay"});
//        m_application.lock()->setActiveScreens({"game"});
//        return;
//    }
}

void SettingsScreen::mousePosEvent(double xpos, double ypos) {

}

void SettingsScreen::mouseButtonEvent(int button, int action) {
    if (Input::tracker.isMouseButtonDown(GLFW_MOUSE_BUTTON_1) && playButton->isMousedOver()) {
        m_application.lock()->setVisibleScreens({"game", "overlay"});
        m_application.lock()->setActiveScreens({"game"});
        Input::tracker.setCursor(false);
        glfwSetInputMode(WindowHandling::handler.getWindowPointer(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        std::cout << "starting game" << std::endl;
    }

    if (Input::tracker.isMouseButtonDown(GLFW_MOUSE_BUTTON_1) && exitButton->isMousedOver()) {
        m_application.lock()->setVisibleScreens({"title"});
        m_application.lock()->setActiveScreens({"title"});
        Input::tracker.setCursor(true);
        glfwSetInputMode(WindowHandling::handler.getWindowPointer(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void SettingsScreen::scrollEvent(double distance) {

}

}
