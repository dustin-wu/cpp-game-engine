#include "guielement.h"

GuiElement::GuiElement(std::string texture, glm::vec2 position, glm::vec2 scale)
{
    texture_ = texture;
    position_ = position;
    scale_ = scale;
    hasText_ = false;
    hasTexture_ = true;
}

GuiElement::GuiElement(std::string texture, glm::vec2 position, glm::vec2 scale, std::shared_ptr<TextInfo> text)
{
    texture_ = texture;
    position_ = position;
    scale_ = scale;
    hasText_ = true;
    text_ = text;
    hasTexture_ = true;
}

GuiElement::GuiElement(glm::vec2 position, glm::vec2 scale, std::shared_ptr<TextInfo> text)
{
    position_ = position;
    scale_ = scale;
    hasText_ = true;
    text_ = text;
    hasTexture_ = false;
}


std::string GuiElement::getTexture() {
    return texture_;
}

glm::vec2 GuiElement::getPosition() {
    return position_;
}

glm::vec2 GuiElement::getScale() {
    return scale_;
}

std::shared_ptr<TextInfo> GuiElement::getText() {
    return text_;
}

