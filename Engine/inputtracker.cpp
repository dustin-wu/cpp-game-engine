#include "inputtracker.h"
#include "GLFW/glfw3.h"


InputTracker::InputTracker()
{
    for (int key = 0; key <= GLFW_KEY_LAST; key++) {
        keyDown[key] = false;
    }
    cursorEnabled_ = true;
}

void InputTracker::setKeyDown(int key, bool down) {
    keyDown[key] = down;
}

bool InputTracker::isKeyDown(int key) {
    return keyDown[key];
}

void InputTracker::setMouseButtonDown(int mouseButton, bool down) {
    mouseButtonDown[mouseButton] = down;
}

bool InputTracker::isMouseButtonDown(int mouseButton) {
    return mouseButtonDown[mouseButton];
}

void InputTracker::setMousePos(float xpos, float ypos) {
    glm::vec2 mousePosVec = glm::vec2(xpos, ypos);
    currentMousePos = mousePosVec;

    if (isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT) || !Input::tracker.cursorEnabled()) {
        glm::vec2 curMousePos = mousePosVec;
        deltaMousePos = curMousePos - prevMousePos;
        prevMousePos = curMousePos;
    }

    if (!isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT) && Input::tracker.cursorEnabled()) {
        prevMousePos = mousePosVec;
    }
}

glm::vec2 InputTracker::getMousePos() {
    return currentMousePos;
}

glm::vec2 InputTracker::getDeltaMousePos() {
    glm::vec2 toReturn = deltaMousePos;
    deltaMousePos = glm::vec2(0, 0);
    return toReturn;
}

void InputTracker::setScrollDistance(float distance) {
    scrollDistance = distance;
}

float InputTracker::getScrollDistance() {
    float toReturn = scrollDistance;
    scrollDistance = 0; // until we scroll again, we want no more scroll
    return toReturn;
}

void InputTracker::setCursor(bool enabled) {
    cursorEnabled_ = enabled;
}

bool InputTracker::cursorEnabled() {
    return cursorEnabled_;
}

