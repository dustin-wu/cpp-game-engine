#pragma once
#include "glm/glm.hpp"
#include <string>

struct TextInfo
{
    TextInfo(std::string text_, std::string font_="opensans", float fontSize_=1.0, glm::vec3 textColor_=glm::vec3(1,1,1), glm::vec2 textOffset_=glm::vec2(0,0))
        : text(text_), font(font_), fontSize(fontSize_), textColor(textColor_), textOffset(textOffset_)
    {}
    std::string text;
    std::string font;
    float fontSize;
    glm::vec3 textColor;
    glm::vec2 textOffset;
};

enum class MaterialType {
    COLOR, TEXTURE
};

class GuiElement
{
public:
    GuiElement(std::string texture, glm::vec2 position, glm::vec2 scale);
    GuiElement(std::string texture, glm::vec2 position, glm::vec2 scale, std::shared_ptr<TextInfo> text);
    GuiElement(glm::vec4 color, glm::vec2 position, glm::vec2 scale);
    GuiElement(glm::vec4 color, glm::vec2 position, glm::vec2 scale, std::shared_ptr<TextInfo> text);
    GuiElement(glm::vec2 position, glm::vec2 scale, std::shared_ptr<TextInfo> text);

    std::string getTexture();
    glm::vec2 getPosition();
    glm::vec2 getScale();
    std::shared_ptr<TextInfo> getText();
    bool hasText() {return hasText_;};
    bool hasTexture() {return hasTexture_;};
    void setText(std::string in_text) {text_->text = in_text; }


protected:
    std::string texture_;
    glm::vec2 position_;
    glm::vec2 scale_;
    std::shared_ptr<TextInfo> text_;
    bool hasText_;
    bool hasTexture_;
    MaterialType materialType_;
};

