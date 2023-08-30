#include "guibutton.h"
#include "Graphics/global.h"
#include "Engine/inputtracker.h"

GuiButton::GuiButton(std::string texture, glm::vec2 position, glm::vec2 scale, std::shared_ptr<TextInfo> text)
    : GuiElement(texture, position, scale, text)
{

}

bool GuiButton::isMousedOver() {
    glm::vec2 mousePos = Input::tracker.getMousePos();
    glm::vec2 windowSize = Global::graphics.getWindowSize();
    std::cout << windowSize.x << " " << windowSize.y << std::endl;
    glm::vec2 positionNegY = glm::vec2(position_.x, -position_.y); // negate Y because GLFW's mouse pos y is in the opposite direction of GUI's
    glm::vec2 position01 = positionNegY / 2.f + glm::vec2(0.5, 0.5); // go from -1,1 to 0,1
    float xMin = (position01.x - scale_.x/2) * windowSize.x;
    float xMax = (position01.x + scale_.x/2) * windowSize.x;
    float yMin = (position01.y - scale_.y/2) * windowSize.y;
    float yMax = (position01.y + scale_.y/2) * windowSize.y;

    return (xMin <= mousePos.x && mousePos.x <= xMax && yMin <= mousePos.y && mousePos.y <= yMax);
}
