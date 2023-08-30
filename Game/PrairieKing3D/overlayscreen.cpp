#include "Game/PrairieKing3D/Systems/enemysystem.h"
#include "Game/PrairieKing3D/Systems/healthsystem.h"
#include "Game/PrairieKing3D/gamescreen.h"
#include "Graphics/global.h"
#include "overlayscreen.h"
#include "Engine/core.h"

namespace PrairieKing3D {

OverlayScreen::OverlayScreen()
    : titleOverlay("Press X to go to title")
    , settingsOverlay("Press O to open settings")
    , startOverlay("Press any key to start")
    , resumeOverlay("Press R to resume")
    , controlsOverlay("WASD to move, space to jump, left click to shoot, R to reload manually, hold and drag right mouse to turn, C to disable cursor, X to exit to title")
    , exitOverlay("Press ESC to exit")
    , victoryOverlay("Press any key to continue")
    , defeatOverlay("Press any key to continue")
{

}

void OverlayScreen::displayEnemyInfo() {
    std::shared_ptr<GameScreen> gameScreen = m_application.lock()->getScreen<GameScreen>("game");
    std::string waveInfo = gameScreen->getGameWorld()->getSystem<EnemySystem>()->getWaveInfo();
    std::string killsInfo = gameScreen->getGameWorld()->getSystem<EnemySystem>()->getKillsInfo();
    std::string healthInfo = gameScreen->getGameWorld()->getSystem<HealthSystem>()->getHealthInfo();
    Global::graphics.bindShader("text");
    Global::graphics.drawUIText(Global::graphics.getFont("opensans"), waveInfo, glm::ivec2(20, 80), AnchorPoint::TopLeft, Global::graphics.getWindowSize().x, 0.5f, 0.1f, glm::vec3(1, 1, 1));
    Global::graphics.drawUIText(Global::graphics.getFont("opensans"), killsInfo, glm::ivec2(20, 50), AnchorPoint::TopLeft, Global::graphics.getWindowSize().x, 0.5f, 0.1f, glm::vec3(1, 1, 1));
    Global::graphics.drawUIText(Global::graphics.getFont("opensans"), healthInfo, glm::ivec2(20, 110), AnchorPoint::TopLeft, Global::graphics.getWindowSize().x, 0.5f, 0.1f, glm::vec3(1, 1, 1));
}

void OverlayScreen::render() {
    Global::graphics.clearScreen(GL_DEPTH_BUFFER_BIT);

    std::vector<std::string> overlayToPrint;
    std::vector<std::string> activeScreens = m_application.lock()->getActiveScreens();
    for (std::string& activeScreen: activeScreens) {
        if (activeScreen == "title") {
            overlayToPrint.push_back(startOverlay);
            overlayToPrint.push_back(exitOverlay);
        }
        if (activeScreen == "settings") {
            overlayToPrint.push_back(controlsOverlay);
            overlayToPrint.push_back(resumeOverlay);
        }
        if (activeScreen == "game") {
            displayEnemyInfo();
//            overlayToPrint.push_back(settingsOverlay);
//            overlayToPrint.push_back(titleOverlay);
        }
        if (activeScreen == "victory") {
            overlayToPrint.push_back(victoryOverlay);
        }
        if (activeScreen == "defeat") {
            overlayToPrint.push_back(defeatOverlay);
        }
    }

    Global::graphics.bindShader("text");
    for (int i = 0; i < overlayToPrint.size(); i++) {
        Global::graphics.drawUIText(Global::graphics.getFont("opensans"), overlayToPrint[i], glm::ivec2(20, 80 - 30 * (overlayToPrint.size() - 1 - i)), AnchorPoint::TopLeft, Global::graphics.getFramebufferSize().x, 0.5f, 0.1f, glm::vec3(1, 1, 1));
    }
}

void OverlayScreen::update(double deltaTime) {

}

void OverlayScreen::windowResizeEvent(int width, int height) {
    Global::graphics.setWindowSize(glm::ivec2(width, height));
}

void OverlayScreen::keyEvent(int key, int action) {

}

void OverlayScreen::mousePosEvent(double xpos, double ypos) {

}

void OverlayScreen::mouseButtonEvent(int button, int action) {

}

void OverlayScreen::scrollEvent(double distance) {

}

}
