#pragma once

#include "Engine/screen.h"
#include "Graphics/GUI/guielement.h"

namespace ExtremeUnaliving {

class GameOverlayScreen : public Screen
{
public:
    GameOverlayScreen();
    void render() override;
    void update(double deltaTime) override;
    void windowResizeEvent(int width, int height) override;
    void keyEvent(int key, int action) override;
    void mousePosEvent(double xpos, double ypos) override;
    void mouseButtonEvent(int button, int action) override;
    void scrollEvent(double distance) override;
private:
    std::shared_ptr<TextInfo> healthBarText;
    std::shared_ptr<GuiElement> healthBar;
    std::shared_ptr<GuiElement> healthBarBackground;
    std::shared_ptr<GuiElement> staminaBarBackground;
    std::shared_ptr<GuiElement> crossHairA;
    std::shared_ptr<GuiElement> crossHairB;
    std::shared_ptr<GuiElement> staminaBar;
    std::shared_ptr<GuiElement> ammo;
    std::shared_ptr<TextInfo> fpsText;
    std::shared_ptr<GuiElement> fps;
    std::shared_ptr<GuiElement> steelImage;
    std::shared_ptr<TextInfo> steelCountText;
    std::shared_ptr<GuiElement> steelCount;
};

}
