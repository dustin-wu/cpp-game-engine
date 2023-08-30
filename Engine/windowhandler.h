#pragma once

#include "GLFW/glfw3.h"



class WindowHandler
{
public:
    WindowHandler();
    void setWindowPointer(GLFWwindow* window_ptr) { m_windowPointer = window_ptr; };
    GLFWwindow* getWindowPointer() { return m_windowPointer; };
private:
    GLFWwindow* m_windowPointer;
};


namespace WindowHandling{
    inline WindowHandler handler;
}
