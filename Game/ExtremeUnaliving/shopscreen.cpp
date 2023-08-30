#include "shopscreen.h"
#include "Game/ExtremeUnaliving/ShopHandler.h"
#include "Game/ExtremeUnaliving/Systems/gameinitializersystem.h"
#include "Game/ExtremeUnaliving/Systems/gunsystem.h"
#include "Graphics/global.h"
#include "Engine/inputtracker.h"
#include "Engine/windowhandler.h"
#include "GLFW/glfw3.h"
#include "glm/fwd.hpp"

namespace ExtremeUnaliving {

ShopScreen::ShopScreen()
{
    ShopItem item1;
    item1.name = "SprayNPray";
    item1.desc = "FR + Ammo up, reload spd down";
    item1.cost = 50;

    ShopItem item2;
    item2.name = "Nanomachines";
    item2.desc = "+5 HP every 10 sec";
    item2.cost = 15;

    ShopItem item3;
    item3.name = "Powerade";
    item3.desc = "Stamina regen up";
    item3.cost = 15;

    ShopItem item4;
    item4.name = "Cyber Eye";
    item4.desc = "Headshots more likely";
    item4.cost = 15;

    ShopItem item5;
    item5.name = "Better Bullets";
    item5.desc = "Bullet dmg +1";
    item5.cost = 10;

    ShopItem item6;
    item6.name = "Technoplate";
    item6.desc = "-2 DMG from all sources";
    item6.cost = 10;

    items = {item1, item2, item3, item4, item5, item6};

    for (int i = 0; i < 3; i++) {
        if (i >= items.size()) break;
        std::shared_ptr<TextInfo> nameText = std::make_shared<TextInfo>(items[i].name, "joystix", 0.5f);
        std::shared_ptr<GuiElement> nameGUI = std::make_shared<GuiElement>(glm::vec2(-0.35, 0.5 - i*0.4), glm::vec2(0.1, 0.1), nameText);

        std::shared_ptr<TextInfo> descText = std::make_shared<TextInfo>(items[i].desc, "joystix", 0.3f);
        std::shared_ptr<GuiElement> descGUI = std::make_shared<GuiElement>(glm::vec2(-0.35, 0.4 - i*0.4), glm::vec2(0.1, 0.1), descText);

        std::shared_ptr<TextInfo> buyText = std::make_shared<TextInfo>(std::to_string(items[i].cost), "joystix", 0.3f);
        std::shared_ptr<GuiButton> buyButton = std::make_shared<GuiButton>("buttonTexture", glm::vec2(-0.35, 0.3 - i*0.4), glm::vec2(0.1, 0.05), buyText);

        std::shared_ptr<GuiElement> iconGUI = std::make_shared<GuiElement>("item" + std::to_string(i+1), glm::vec2(-0.6, 0.4 - i*0.4), glm::vec2(0.0625, 0.1));


        items[i].nameGUI = nameGUI;
        items[i].descGUI = descGUI;
        items[i].buyButton = buyButton;
        items[i].iconGUI = iconGUI;
    }

    for (int i = 3; i < 6; i++) {
        if (i >= items.size()) break;
        std::shared_ptr<TextInfo> nameText = std::make_shared<TextInfo>(items[i].name, "joystix", 0.5f);
        std::shared_ptr<GuiElement> nameGUI = std::make_shared<GuiElement>(glm::vec2(0.4, 0.5 - (i-3)*0.4), glm::vec2(0.1, 0.1), nameText);

        std::shared_ptr<TextInfo> descText = std::make_shared<TextInfo>(items[i].desc, "joystix", 0.3f);
        std::shared_ptr<GuiElement> descGUI = std::make_shared<GuiElement>(glm::vec2(0.4, 0.4 - (i-3)*0.4), glm::vec2(0.1, 0.1), descText);

        std::shared_ptr<TextInfo> buyText = std::make_shared<TextInfo>(std::to_string(items[i].cost), "joystix", 0.3f);
        std::shared_ptr<GuiButton> buyButton = std::make_shared<GuiButton>("buttonTexture", glm::vec2(0.4, 0.3 - (i-3)*0.4), glm::vec2(0.1, 0.05), buyText);

        std::shared_ptr<GuiElement> iconGUI = std::make_shared<GuiElement>("item" + std::to_string(i+1), glm::vec2(0.15, 0.4 - (i-3)*0.4), glm::vec2(0.0625, 0.1));

        items[i].nameGUI = nameGUI;
        items[i].descGUI = descGUI;
        items[i].buyButton = buyButton;
        items[i].iconGUI = iconGUI;
    }

}

void ShopScreen::render() {
    Global::graphics.setClearColor(glm::vec3(0, 0, 0));

    Global::graphics.bindShader("gui");
    background = std::make_shared<GuiElement>("barbackgroundTexture", glm::vec2(0, 0), glm::vec2(0.7, 0.7));
    Global::graphics.drawGUI(background, GUIAnchorPoint::CENTER);

    for (int i = 0; i < 6; i++) {
        if (i >= items.size()) break;
        Global::graphics.drawGUI(items[i].nameGUI, GUIAnchorPoint::CENTER);
        Global::graphics.drawGUI(items[i].descGUI, GUIAnchorPoint::CENTER);
        Global::graphics.drawGUI(items[i].buyButton, GUIAnchorPoint::CENTER);
        Global::graphics.drawGUI(items[i].iconGUI, GUIAnchorPoint::CENTER);
    }

}

void ShopScreen::update(double deltaTime) {

}

void ShopScreen::windowResizeEvent(int width, int height) {
    Global::graphics.setWindowSize(glm::ivec2(width, height));
}

void ShopScreen::keyEvent(int key, int action) {
}

void ShopScreen::mousePosEvent(double xpos, double ypos) {

}

void ShopScreen::buyIfClicked(int itemIndex) {
    if (items[itemIndex].buyButton->isMousedOver() && !ShopHandling::handler.itemBought(itemIndex) && ShopHandling::handler.numSteel >= items[itemIndex].cost) {
        items[itemIndex].buyButton->setText("sold out!");
        ShopHandling::handler.numSteel -= items[itemIndex].cost;
        ShopHandling::handler.setItemBought(itemIndex);
    }
}

void ShopScreen::mouseButtonEvent(int button, int action) {
    if (!Input::tracker.isMouseButtonDown(GLFW_MOUSE_BUTTON_1)) return;

    for (int itemIndex = 0; itemIndex < 6; itemIndex++) {
        buyIfClicked(itemIndex);
    }
}

void ShopScreen::scrollEvent(double distance) {

}



}
