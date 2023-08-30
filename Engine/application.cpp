#include "Engine/inputtracker.h"
#include "Engine/screen.h"
#include "core.h"
#include "OpenGL/OpenGL.h"
#include <algorithm>
#include <iostream>
#include "application.h"

Application::Application()
    : m_gameWorld(std::make_shared<GameWorld>())
{
}

void Application::addScreen(std::string screenName, std::shared_ptr<Screen> screen) {
    screen->setApplication(shared_from_this());
    m_screens[screenName] = screen;
    screenNames.insert(screenName);
}

void Application::setVisibleScreens(std::initializer_list<std::string> screens) {
    curVisibleScreens.clear();
    // Only add screens that have been registered
    for (const std::string& screenName: screens) {
        if (screenNames.count(screenName)) {
            curVisibleScreens.push_back(screenName);
        }
    }
}

std::vector<std::string> Application::getActiveScreens() {
    return curActiveScreens;
}

void Application::setActiveScreens(std::initializer_list<std::string> screens) {
    curActiveScreens.clear();
    // Only add screens that have been registered
    for (const std::string&  screenName: screens) {
        if (screenNames.count(screenName)) {
            curActiveScreens.push_back(screenName);
        }
    }
}

void Application::render() {
    for (std::string& screenName: curVisibleScreens) {
        m_screens[screenName]->render();
    }
}

void Application::update(double deltaTime) {
    for (std::string& screenName: curActiveScreens) {
        m_screens[screenName]->update(deltaTime);
    }
}

void Application::windowResizeEvent(int width, int height) {
    for (const std::string& screenName: screenNames) {
        m_screens[screenName]->windowResizeEvent(width, height);
    }
}

void Application::keyEvent(int key, int action) {
    Input::tracker.setKeyDown(key, action);

    for (std::string& screenName: curActiveScreens) {
        m_screens[screenName]->keyEvent(key, action);
    }
}

void Application::mousePosEvent(double xpos, double ypos) {
    for (std::string& screenName: curActiveScreens) {
        m_screens[screenName]->mousePosEvent(xpos, ypos);
    }
}

void Application::mouseButtonEvent(int button, int action) {
    for (std::string& screenName: curActiveScreens) {
        m_screens[screenName]->mouseButtonEvent(button, action);
    }
}

void Application::scrollEvent(double distance) {
    for (std::string& screenName: curVisibleScreens) {
        m_screens[screenName]->scrollEvent(distance);
    }
}


