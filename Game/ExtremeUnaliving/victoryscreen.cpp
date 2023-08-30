#include "Engine/inputtracker.h"
#include "Game/ExtremeUnaliving/gamescreen.h"
#include "Graphics/global.h"
#include "VictoryScreen.h"
#include "GLFW/glfw3.h"
#include "Engine/windowhandler.h"


namespace ExtremeUnaliving {

VictoryScreen::VictoryScreen()
{
    std::shared_ptr<TextInfo> winText = std::make_shared<TextInfo>("You Win!", "joystix");
    title = std::make_shared<GuiElement>(glm::vec2(0, 0.5), glm::vec2(0.6, 0.75), winText);

    std::shared_ptr<TextInfo> winText1 = std::make_shared<TextInfo>("Thanks for playing my game", "joystix");
    title1 = std::make_shared<GuiElement>(glm::vec2(0, 0.3), glm::vec2(0.6, 0.75), winText1);

    std::shared_ptr<TextInfo> winText2 = std::make_shared<TextInfo>("And a special thanks to Logan and Daniel", "joystix");
    title2 = std::make_shared<GuiElement>(glm::vec2(-0.2, 0.1), glm::vec2(0.6, 0.75), winText2);

    std::shared_ptr<TextInfo> winText3 = std::make_shared<TextInfo>("for an awesome semester :)", "joystix");
    title3 = std::make_shared<GuiElement>(glm::vec2(-0.2, -0.1), glm::vec2(0.6, 0.75), winText3);

    playButtonText = std::make_shared<TextInfo>("Title", "joystix");
    playButton = std::make_shared<GuiButton>("buttonTexture", glm::vec2(0, -0.3), glm::vec2(0.15, 0.075), playButtonText);

    exitButtonText = std::make_shared<TextInfo>("Quit", "joystix");
    exitButton = std::make_shared<GuiButton>("buttonTexture", glm::vec2(0, -0.7), glm::vec2(0.15, 0.075), exitButtonText);

    justExited = false;
}

void VictoryScreen::VictoryScreen::render() {
    Global::graphics.setClearColor(glm::vec3(0, 0, 0));
    Global::graphics.clearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Global::graphics.bindShader("text");


    Global::graphics.bindShader("gui");
    Global::graphics.drawGUI(title, GUIAnchorPoint::CENTER);
    Global::graphics.drawGUI(title1, GUIAnchorPoint::CENTER);
    Global::graphics.drawGUI(title2, GUIAnchorPoint::CENTER);
    Global::graphics.drawGUI(title3, GUIAnchorPoint::CENTER);

    Global::graphics.drawGUI(playButton, GUIAnchorPoint::CENTER);
    Global::graphics.drawGUI(exitButton, GUIAnchorPoint::CENTER);
}

void VictoryScreen::update(double deltaTime) {

}

void VictoryScreen::windowResizeEvent(int width, int height) {
    Global::graphics.setWindowSize(glm::ivec2(width, height));
}

void VictoryScreen::keyEvent(int key, int action) {
//    if (action == GLFW_PRESS) {
//        // Press any key to start
////        m_application.lock()->getScreen<GameScreen>("game")->resetLevel();
////        m_application.lock()->getScreen<GameScreen>("game")->startGame();
//        m_application.lock()->setVisibleScreens({"game", "overlay"});
//        m_application.lock()->setActiveScreens({"game"});
//        return;
//    }
}

void VictoryScreen::mousePosEvent(double xpos, double ypos) {

}

void VictoryScreen::mouseButtonEvent(int button, int action) {
    if (Input::tracker.isMouseButtonDown(GLFW_MOUSE_BUTTON_1) && playButton->isMousedOver()) {
        m_application.lock()->setVisibleScreens({"title"});
        m_application.lock()->setActiveScreens({"title"});
        Input::tracker.setCursor(true);
        glfwSetInputMode(WindowHandling::handler.getWindowPointer(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if (Input::tracker.isMouseButtonDown(GLFW_MOUSE_BUTTON_1) && exitButton->isMousedOver()) {
        std::cout << "closing window" << std::endl;
        glfwSetWindowShouldClose(WindowHandling::handler.getWindowPointer(), true);
    }
}

void VictoryScreen::scrollEvent(double distance) {

}

}
