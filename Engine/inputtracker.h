#pragma once

#include <unordered_map>
#include "glm/glm.hpp"
#include "glm/ext.hpp"

class InputTracker
{
public:
    InputTracker();
    void setKeyDown(int key, bool down);
    bool isKeyDown(int key);
    void setMouseButtonDown(int mouseButton, bool down);
    bool isMouseButtonDown(int mouseButton);
    void setMousePos(float xpos, float ypos);
    glm::vec2 getMousePos();
    glm::vec2 getDeltaMousePos();
    void setScrollDistance(float distance);
    float getScrollDistance();
    void setCursor(bool enabled);
    bool cursorEnabled();

private:
    std::unordered_map<int, bool> keyDown;
    std::unordered_map<int, bool> mouseButtonDown;
    glm::vec2 prevMousePos;
    glm::vec2 deltaMousePos;
    glm::vec2 currentMousePos;
    float scrollDistance;
    bool cursorEnabled_;
    void* m_window;
};

namespace Input{
    inline InputTracker tracker;
}

