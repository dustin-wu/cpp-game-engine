#include "guirenderer.h"
#include "Graphics/global.h"
#include "Graphics/material.h"

void GuiRenderer::initialize(){
    std::cout << "gui initialized" << std::endl;
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GuiRenderer::renderGUIElement(std::shared_ptr<GuiElement> guiElement, GUIAnchorPoint anchor) {
    glBindVertexArray(m_vao);

    glm::ivec2 windowSize = Global::graphics.getWindowSize();
    float xpos = (guiElement->getPosition().x * 0.5f + 0.5f) * windowSize.x;
    float ypos = (guiElement->getPosition().y * 0.5f + 0.5f) * windowSize.y;
    float w = guiElement->getScale().x * windowSize.x;
    float h = guiElement->getScale().y * windowSize.y;

    float xLeft;
    float xRight;

    switch (anchor) {
    case GUIAnchorPoint::CENTER:
        xLeft = xpos - w/2;
        xRight = xpos + w/2;
        break;

    case GUIAnchorPoint::LEFT:
        xLeft = xpos;
        xRight = xpos + w;
        break;

    case GUIAnchorPoint::RIGHT:
        xLeft = xpos - w;
        xRight = xpos;
        break;
    }

    float vertices[6][4] = {
        { xLeft,   ypos + h/2,   0.0f, 1.0f },
        { xLeft,   ypos - h/2,   0.0f, 0.0f },
        { xRight,   ypos - h/2,   1.0f, 0.0f },

        { xLeft,   ypos + h/2,   0.0f, 1.0f },
        { xRight,   ypos - h/2,   1.0f, 0.0f },
        { xRight,   ypos + h/2,   1.0f, 1.0f }
    };
    glDisable(GL_DEPTH_TEST);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
}
