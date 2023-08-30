#pragma once


#include "Graphics/GUI/guielement.h"

class GuiButton : public GuiElement
{
public:
    GuiButton(std::string texture, glm::vec2 position, glm::vec2 scale, std::shared_ptr<TextInfo> text);
    bool isMousedOver();

private:
};

