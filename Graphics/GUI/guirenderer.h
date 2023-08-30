#pragma once

#include <GL/glew.h>
#include "Graphics/GUI/guielement.h"

enum class GUIAnchorPoint{
    LEFT, CENTER, RIGHT
};

class GuiRenderer
{
public:
    void initialize();
    void renderGUIElement(std::shared_ptr<GuiElement> guiElement, GUIAnchorPoint anchor=GUIAnchorPoint::CENTER);

private:
    GLuint m_vao;
    GLuint m_vbo;
};

