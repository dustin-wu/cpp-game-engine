#pragma once

#include "Engine/screen.h"
#include "Graphics/GUI/guibutton.h"
#include "Graphics/GUI/guielement.h"

namespace ExtremeUnaliving {

class DefeatScreen : public Screen
{
public:
    DefeatScreen();
    void render() override;
    void update(double deltaTime) override;
    void windowResizeEvent(int width, int height) override;
    void keyEvent(int key, int action) override;
    void mousePosEvent(double xpos, double ypos) override;
    void mouseButtonEvent(int button, int action) override;
    void scrollEvent(double distance) override;

private:
    std::shared_ptr<GuiElement> title;

    std::shared_ptr<TextInfo> playButtonText;
    std::shared_ptr<GuiButton> playButton;
};


}
