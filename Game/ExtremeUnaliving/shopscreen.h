#pragma once

#include "Engine/screen.h"
#include "Graphics/GUI/guibutton.h"
#include "Graphics/GUI/guielement.h"
namespace ExtremeUnaliving {

struct ShopItem{
    int cost;
    std::string name;
    std::string desc;
    std::string icon;
    bool sold;
    std::function<void()> onBuy;
    std::shared_ptr<GuiElement> nameGUI;
    std::shared_ptr<GuiElement> descGUI;
    std::shared_ptr<GuiElement> iconGUI;
    std::shared_ptr<GuiButton> buyButton;
};

class ShopScreen : public Screen
{
public:
    ShopScreen();
    void render() override;
    void update(double deltaTime) override;
    void windowResizeEvent(int width, int height) override;
    void keyEvent(int key, int action) override;
    void mousePosEvent(double xpos, double ypos) override;
    void mouseButtonEvent(int button, int action) override;
    void scrollEvent(double distance) override;

private:
    void buyIfClicked(int itemIndex);
    std::vector<ShopItem> items;
    std::shared_ptr<TextInfo> costText;
    std::shared_ptr<GuiButton> buyButton;
    std::shared_ptr<GuiElement> icon;
    std::shared_ptr<TextInfo> descText;
    std::shared_ptr<TextInfo> nameText;


    std::shared_ptr<GuiElement> background;
};



}
