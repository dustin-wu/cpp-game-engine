#include "Graphics/GUI/guibutton.h"
#include "Graphics/GUI/guielement.h"
#include <Engine/screen.h>

#pragma once

namespace ExtremeUnaliving {

class TitleScreen : public Screen
{
public:
    TitleScreen();
    void render() override;
    void update(double deltaTime) override;
    void windowResizeEvent(int width, int height) override;
    void keyEvent(int key, int action) override;
    void mousePosEvent(double xpos, double ypos) override;
    void mouseButtonEvent(int button, int action) override;
    void scrollEvent(double distance) override;
    bool justExited;
private:
    std::shared_ptr<GuiElement> title;

    std::shared_ptr<TextInfo> playButtonText;
    std::shared_ptr<GuiButton> playButton;

    std::shared_ptr<TextInfo> exitButtonText;
    std::shared_ptr<GuiButton> exitButton;
};

}
