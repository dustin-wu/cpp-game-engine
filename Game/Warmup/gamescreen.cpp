#include "gamescreen.h"
#include "Engine/Systems/camerasystem.h"
#include "Engine/Systems/charactercontrollersystem.h"
#include "Engine/Systems/collisionsystem.h"
#include "Engine/Systems/drawsystem.h"
#include "Engine/Systems/physicssystem.h"
#include "GLFW/glfw3.h"
#include "Graphics/global.h"

GameScreen::GameScreen()
{}

void GameScreen::render() {
    Global::graphics.setClearColor(glm::vec3(0, 0.5, 1));
    Global::graphics.clearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_application.lock()->getGameWorld()->getSystem<DrawSystem>()->draw();
}

void GameScreen::update(double deltaTime) {
    m_application.lock()->getGameWorld()->getSystem<CharacterControllerSystem>()->updateCharacterTransform(deltaTime);
    m_application.lock()->getGameWorld()->getSystem<PhysicsSystem>()->applyKinematics(deltaTime);
    m_application.lock()->getGameWorld()->getSystem<CollisionSystem>()->updateAllCollisions();
    m_application.lock()->getGameWorld()->getSystem<CollisionSystem>()->resolveCollisions();
    m_application.lock()->getGameWorld()->getSystem<CameraSystem>()->followCharacter();
}

void GameScreen::windowResizeEvent(int width, int height) {
    Global::graphics.setWindowSize(glm::ivec2(width, height));
    m_application.lock()->getGameWorld()->getSystem<CameraSystem>()->resizeCamera(width, height);
}

void GameScreen::keyEvent(int key, int action) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_T) {
            m_application.lock()->getGameWorld()->reset();
            m_application.lock()->setVisibleScreens({"title", "hints"});
            m_application.lock()->setActiveScreens({"title"});
        }
        else if (key == GLFW_KEY_R) {
            m_application.lock()->setVisibleScreens({"game", "settings", "hints"});
            m_application.lock()->setActiveScreens({"settings"});
        }
    }
}

void GameScreen::mousePosEvent(double xpos, double ypos) {

}

void GameScreen::mouseButtonEvent(int button, int action) {

}

void GameScreen::scrollEvent(double distance) {

}

