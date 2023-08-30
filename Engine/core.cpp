#include "core.h"
#include "Engine/inputtracker.h"
#include "Game/Warmup/warmupapplication.h"
#include "Game/Testing/gameapplication.h"
#include "Game/PrairieKing3D/gameapplication.h"
#include "Game/ExtremeUnaliving/gameapplication.h"

//using namespace PrairieKing3D; // change this line to change the game!
//using namespace Testing; // change this line to change the game!
using namespace ExtremeUnaliving;

Core::Core()
    : m_application(std::make_shared<GameApplication>())
{
    m_application->initializeApp();
}

Core::~Core(){

}

void Core::update(double deltaTime){
    m_application->update(deltaTime);
}

void Core::draw(){
    m_application->render();
}

void Core::keyEvent(int key, int action){
    Input::tracker.setKeyDown(key, action);
    m_application->keyEvent(key, action);
}

void Core::mousePosEvent(double xpos, double ypos){
    Input::tracker.setMousePos(xpos, ypos);
    m_application->mousePosEvent(xpos, ypos);
}

void Core::mouseButtonEvent(int button, int action){
    Input::tracker.setMouseButtonDown(button, action);
    m_application->mouseButtonEvent(button, action);
}

void Core::scrollEvent(double distance){
    Input::tracker.setScrollDistance(distance);
    m_application->scrollEvent(distance);
}

void Core::framebufferResizeEvent(int width, int height){
    Global::graphics.setFramebufferSize(glm::ivec2(width, height));
}

void Core::windowResizeEvent(int width, int height){
    m_application->windowResizeEvent(width, height);
}
