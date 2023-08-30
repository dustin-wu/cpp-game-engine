#pragma once

#include "Engine/gameworld.h"
#include <map>
#include <vector>
#include <unordered_set>
#include <string>

// Forward declaration so that Application knows about Screen and vice-versa
class Screen;

class Application : public std::enable_shared_from_this<Application>
{
public:
    Application();
    ~Application(){}
    virtual void addScreen(std::string screenName, std::shared_ptr<Screen> screen);
    virtual void setVisibleScreens(std::initializer_list<std::string> screens);
    virtual void setActiveScreens(std::initializer_list<std::string> screens);
    template <class T>
    std::shared_ptr<T> getScreen(std::string screenName) { return std::dynamic_pointer_cast<T>(m_screens[screenName]); }
    virtual std::vector<std::string> getActiveScreens();
    virtual void render();
    virtual void update(double deltaTime);
    virtual void windowResizeEvent(int width, int height);
    virtual void keyEvent(int key, int action);
    virtual void mousePosEvent(double xpos, double ypos);
    virtual void mouseButtonEvent(int button, int action);
    virtual void scrollEvent(double distance);
    virtual void initializeApp() = 0;
    std::shared_ptr<GameWorld> getGameWorld() { return m_gameWorld; }

protected:
    // Collection of all screens that the application is managing
    std::map<std::string, std::shared_ptr<Screen>> m_screens;

    // Vector of screens to render to the window; screens with greater priority
    // are rendered last and appear on top of those with lesser priority
    std::vector<std::string> curVisibleScreens;

    // Vector of screens to be allowed to update and process input
    std::vector<std::string> curActiveScreens;

    // Every screen name in the collection
    std::unordered_set<std::string> screenNames;

    std::shared_ptr<GameWorld> m_gameWorld; // Only used by warmup 1 game
};


