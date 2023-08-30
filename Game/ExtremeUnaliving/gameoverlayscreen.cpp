#include "Game/ExtremeUnaliving/ShopHandler.h"
#include "Game/ExtremeUnaliving/Systems/enemysystem.h"
#include "Game/ExtremeUnaliving/Systems/gunsystem.h"
#include "Game/ExtremeUnaliving/Systems/healthsystem.h"
#include "Game/ExtremeUnaliving/Systems/specialcharactercontrollersystem.h"
#include "Game/ExtremeUnaliving/gamescreen.h"
#include "Graphics/global.h"
#include "gameoverlayscreen.h"
#include "Engine/core.h"

namespace ExtremeUnaliving {

GameOverlayScreen::GameOverlayScreen()
{
    crossHairA = std::make_shared<GuiElement>("white", glm::vec2(0, 0), glm::vec2(0.0065, 0.001));
    crossHairB = std::make_shared<GuiElement>("white", glm::vec2(0, 0), glm::vec2(0.001, 0.01));
    steelImage = std::make_shared<GuiElement>("steel", glm::vec2(0.9, -0.8), glm::vec2(0.0625, 0.1));
}


void GameOverlayScreen::render() {
    Global::graphics.clearScreen(GL_DEPTH_BUFFER_BIT);
    Global::graphics.bindShader("gui");
    std::shared_ptr<GameScreen> gameScreen = m_application.lock()->getScreen<GameScreen>("game");
    int health = gameScreen->getGameWorld()->getSystem<HealthSystem>()->getHealth();
    healthBarText = std::make_shared<TextInfo>(std::to_string(health), "joystix");
    healthBarText->textOffset = glm::vec2(0.1, 0);
    healthBarBackground = std::make_shared<GuiElement>("barbackgroundTexture", glm::vec2(-0.96, -0.8), glm::vec2(0.31, 0.06));

    healthBar = std::make_shared<GuiElement>("healthbarTexture", glm::vec2(-0.95, -0.8), glm::vec2(0.3 * (float)health/100, 0.05), healthBarText);
    Global::graphics.drawGUI(healthBarBackground, GUIAnchorPoint::LEFT);
    Global::graphics.drawGUI(healthBar, GUIAnchorPoint::LEFT);
    Global::graphics.drawGUI(crossHairA, GUIAnchorPoint::CENTER);
    Global::graphics.drawGUI(crossHairB, GUIAnchorPoint::CENTER);

    float stamina = gameScreen->getGameWorld()->getSystem<SpecialCharacterControllerSystem>()->getStamina();
    int whole = floor(stamina);
    float partial = stamina - whole;
    float xOffset = 0;
    staminaBarBackground = std::make_shared<GuiElement>("barbackgroundTexture", glm::vec2(-0.96, -0.67), glm::vec2(0.31, 0.06));
    Global::graphics.drawGUI(staminaBarBackground, GUIAnchorPoint::LEFT);
    for (int i = 0; i < whole; i++) {
        staminaBar = std::make_shared<GuiElement>("staminaTexture", glm::vec2(-0.95 + xOffset, -0.67), glm::vec2(0.096, 0.05));
        Global::graphics.drawGUI(staminaBar, GUIAnchorPoint::LEFT);
        xOffset += 0.01 + 0.193;
    }
    staminaBar = std::make_shared<GuiElement>("staminaTexture", glm::vec2(-0.95 + xOffset, -0.67), glm::vec2(0.096 * partial, 0.05));
    Global::graphics.drawGUI(staminaBar, GUIAnchorPoint::LEFT);

    int ammoCount = gameScreen->getGameWorld()->getSystem<GunSystem>()->getAmmoCount();
    xOffset = 0;
    for (int i = 0; i < ammoCount; i++) {
        ammo = std::make_shared<GuiElement>("ammoTexture", glm::vec2(-0.95 + xOffset, -0.55), glm::vec2(0.03, 0.05));
        xOffset += 0.04;
        Global::graphics.drawGUI(ammo, GUIAnchorPoint::LEFT);
    }

    steelCountText = std::make_shared<TextInfo>("x " + std::to_string(ShopHandling::handler.numSteel), "joystix", 0.5f);
    steelCount = std::make_shared<GuiElement>(glm::vec2(0.9, -0.9), glm::vec2(0.03, 0.05), steelCountText);

    Global::graphics.drawGUI(steelImage, GUIAnchorPoint::CENTER);
    Global::graphics.drawGUI(steelCount, GUIAnchorPoint::CENTER);

}

void GameOverlayScreen::update(double deltaTime) {

}

void GameOverlayScreen::windowResizeEvent(int width, int height) {
    Global::graphics.setWindowSize(glm::ivec2(width, height));
}

void GameOverlayScreen::keyEvent(int key, int action) {

}

void GameOverlayScreen::mousePosEvent(double xpos, double ypos) {

}

void GameOverlayScreen::mouseButtonEvent(int button, int action) {

}

void GameOverlayScreen::scrollEvent(double distance) {

}

}
