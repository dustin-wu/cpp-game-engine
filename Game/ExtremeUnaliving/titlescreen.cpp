#include "Engine/inputtracker.h"
#include "Game/ExtremeUnaliving/gamescreen.h"
#include "Graphics/global.h"
#include "titlescreen.h"
#include "GLFW/glfw3.h"
#include "Engine/windowhandler.h"


namespace ExtremeUnaliving {

TitleScreen::TitleScreen()
{

    title = std::make_shared<GuiElement>("title", glm::vec2(0, 0.3), glm::vec2(0.6, 0.75));

    playButtonText = std::make_shared<TextInfo>("Play", "joystix");
    playButton = std::make_shared<GuiButton>("buttonTexture", glm::vec2(0, -0.3), glm::vec2(0.15, 0.075), playButtonText);

    exitButtonText = std::make_shared<TextInfo>("Quit", "joystix");
    exitButton = std::make_shared<GuiButton>("buttonTexture", glm::vec2(0, -0.7), glm::vec2(0.15, 0.075), exitButtonText);

    justExited = false;
}

void TitleScreen::TitleScreen::render() {
    Global::graphics.setClearColor(glm::vec3(0, 0, 0));
    Global::graphics.clearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Global::graphics.bindShader("text");

//    Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Steel and Spurs", glm::ivec2(20, 420), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 1.5f, 0.5f, glm::vec3(1, 1, 1));

    Global::graphics.bindShader("gui");
    Global::graphics.drawGUI(title, GUIAnchorPoint::CENTER);

    Global::graphics.drawGUI(playButton, GUIAnchorPoint::CENTER);
    Global::graphics.drawGUI(exitButton, GUIAnchorPoint::CENTER);
}

void TitleScreen::update(double deltaTime) {

}

void TitleScreen::windowResizeEvent(int width, int height) {
    Global::graphics.setWindowSize(glm::ivec2(width, height));
}

void TitleScreen::keyEvent(int key, int action) {
//    if (action == GLFW_PRESS) {
//        // Press any key to start
////        m_application.lock()->getScreen<GameScreen>("game")->resetLevel();
////        m_application.lock()->getScreen<GameScreen>("game")->startGame();
//        m_application.lock()->setVisibleScreens({"game", "overlay"});
//        m_application.lock()->setActiveScreens({"game"});
//        return;
//    }
}

void TitleScreen::mousePosEvent(double xpos, double ypos) {

}

void TitleScreen::mouseButtonEvent(int button, int action) {
    if (Input::tracker.isMouseButtonDown(GLFW_MOUSE_BUTTON_1) && playButton->isMousedOver()) {
        m_application.lock()->getScreen<GameScreen>("game")->startGame();
        m_application.lock()->setVisibleScreens({"game", "overlay"});
        m_application.lock()->setActiveScreens({"game"});
        Input::tracker.setCursor(false);
        glfwSetInputMode(WindowHandling::handler.getWindowPointer(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        std::cout << "starting game" << std::endl;
        justExited = true;
    }

    if (Input::tracker.isMouseButtonDown(GLFW_MOUSE_BUTTON_1) && exitButton->isMousedOver()) {
        std::cout << "closing window" << std::endl;
        glfwSetWindowShouldClose(WindowHandling::handler.getWindowPointer(), true);
    }
}

void TitleScreen::scrollEvent(double distance) {

}

}
