#include "gamescreen.h"
#include "Engine/Systems/camerasystem.h"
#include "Engine/Systems/charactercontrollersystem.h"
#include "Engine/Systems/collisionsystem.h"
#include "Engine/Systems/drawsystem.h"
#include "Engine/Systems/physicssystem.h"
#include "GLFW/glfw3.h"
#include "Game/PrairieKing3D/Systems/bulletsystem.h"
#include "Game/PrairieKing3D/Systems/enemysystem.h"
#include "Game/PrairieKing3D/Systems/gunsystem.h"
#include "Game/PrairieKing3D/Systems/healthsystem.h"
#include "Game/PrairieKing3D/Systems/initializersystem.h"
#include "Graphics/global.h"

namespace PrairieKing3D {

GameScreen::GameScreen()
    : m_gameWorld(std::make_shared<GameWorld>())
{
    m_gameWorld->addSystem<InitializerSystem>();
    m_gameWorld->getSystem<InitializerSystem>()->initalizeGame();
}

void GameScreen::startGame() {
    std::string fileName = "Game/PrairieKing3D/Maps/level" + std::to_string(currentLevel) + ".txt";
    m_gameWorld->getSystem<InitializerSystem>()->loadGame(fileName);
}

void GameScreen::render() {
    Global::graphics.setClearColor(glm::vec3(0, 0.5, 1));
    Global::graphics.clearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_gameWorld->getSystem<DrawSystem>()->draw();
}

void GameScreen::update(double deltaTime) {
    if (m_gameWorld->getSystem<EnemySystem>()->won()) {
        std::cout << "Victory!" << std::endl;
        m_application.lock()->setVisibleScreens({"victory", "overlay"});
        m_application.lock()->setActiveScreens({"victory"});
        return;
    }

    if (m_gameWorld->getSystem<HealthSystem>()->getHealth() == 0) {
        m_application.lock()->setVisibleScreens({"defeat", "overlay"});
        m_application.lock()->setActiveScreens({"defeat"});
        return;
    }

    m_gameWorld->getSystem<CharacterControllerSystem>()->updateCharacterTransform(deltaTime);
    m_gameWorld->getSystem<PhysicsSystem>()->applyKinematics(deltaTime);
    m_gameWorld->getSystem<CollisionSystem>()->updateAllCollisions();
    m_gameWorld->getSystem<CollisionSystem>()->resolveCollisions();
    m_gameWorld->getSystem<CameraSystem>()->followCharacter();
    m_gameWorld->getSystem<GunSystem>()->updateGunTransform(deltaTime);
    m_gameWorld->getSystem<GunSystem>()->fireBulletOnMouse(deltaTime);
    m_gameWorld->getSystem<EnemySystem>()->updateEnemies(deltaTime);
    m_gameWorld->getSystem<EnemySystem>()->spawnEnemies(deltaTime);
    m_gameWorld->getSystem<BulletSystem>()->trackBullets();
}

void GameScreen::windowResizeEvent(int width, int height) {
    Global::graphics.setWindowSize(glm::ivec2(width, height));
    m_gameWorld->getSystem<CameraSystem>()->resizeCamera(width, height);
}

void GameScreen::keyEvent(int key, int action) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_X) {
            m_gameWorld->reset();
            m_application.lock()->setVisibleScreens({"title", "overlay"});
            m_application.lock()->setActiveScreens({"title"});
        }
        else if (key == GLFW_KEY_O) {
            m_application.lock()->setVisibleScreens({"game", "settings", "overlay"});
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

}
