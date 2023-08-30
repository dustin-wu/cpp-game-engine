#include "gamescreen.h"
#include "Engine/Systems/aisystem.h"
#include "Engine/Systems/camerasystem.h"
#include "Engine/Systems/collisionsystem.h"
#include "Engine/Systems/drawanimationsystem.h"
#include "Engine/Systems/drawsystem.h"
#include "Engine/Systems/physicssystem.h"
#include "Engine/Systems/uniformgridsystem.h"
#include "Engine/inputtracker.h"
#include "Game/ExtremeUnaliving/Systems/bulletsystem.h"
#include "Game/ExtremeUnaliving/Systems/checkpointsystem.h"
#include "Game/ExtremeUnaliving/Systems/doorsystem.h"
#include "Game/ExtremeUnaliving/Systems/shopsystem.h"
#include "Game/ExtremeUnaliving/Systems/enemysystem.h"
#include "Game/ExtremeUnaliving/Systems/gunsystem.h"
#include "Game/ExtremeUnaliving/Systems/healthsystem.h"
#include "Game/ExtremeUnaliving/Systems/jumppadsystem.h"
#include "Game/ExtremeUnaliving/Systems/spawnersystem.h"
#include "Game/ExtremeUnaliving/Systems/specialcharactercontrollersystem.h"
#include "Game/ExtremeUnaliving/Systems/trophysystem.h"
#include "Graphics/global.h"
#include "glm/fwd.hpp"
#include "Game/ExtremeUnaliving/Systems/gameinitializersystem.h"
#include "GLFW/glfw3.h"
#include "Engine/windowhandler.h"

namespace ExtremeUnaliving {

GameScreen::GameScreen()
    : m_gameWorld(std::make_shared<GameWorld>())
{
    m_gameWorld->addSystem<GameInitializerSystem>();
    m_gameWorld->getSystem<GameInitializerSystem>()->InitializeGame();
}

void GameScreen::startGame(bool resetCheckpoints) {
    m_gameWorld->reset();
    m_gameWorld->getSystem<GameInitializerSystem>()->startGame(resetCheckpoints);
}

void GameScreen::render() {
    Global::graphics.setClearColor(glm::vec3(0, 0, 0));
    Global::graphics.clearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_gameWorld->getSystem<DrawSystem>()->draw();
    m_gameWorld->getSystem<DrawAnimationSystem>()->draw();

    m_gameWorld->getSystem<GunSystem>()->drawGunfire();
    Global::particleMaster.setCamera(m_gameWorld->getSystem<CameraSystem>()->getActiveCamera());
    Global::particleMaster.renderParticles();
}

void GameScreen::update(double deltaTime) {

    if (m_gameWorld->getSystem<HealthSystem>()->getPlayerHealth() <= 0) {
        Input::tracker.setCursor(true);
        glfwSetInputMode(WindowHandling::handler.getWindowPointer(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        m_application.lock()->setVisibleScreens({"defeat"});
        m_application.lock()->setActiveScreens({"defeat"});
        return;
    }

    if (m_gameWorld->getSystem<ShopSystem>()->checkForShops(0)) {
        m_application.lock()->setVisibleScreens({"game", "overlay", "shop"});
        m_application.lock()->setActiveScreens({"game", "shop"});
        Input::tracker.setCursor(true);
        glfwSetInputMode(WindowHandling::handler.getWindowPointer(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
        m_application.lock()->setVisibleScreens({"game", "overlay"});
        m_application.lock()->setActiveScreens({"game"});
        Input::tracker.setCursor(false);
        glfwSetInputMode(WindowHandling::handler.getWindowPointer(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    if (m_gameWorld->getSystem<TrophySystem>()->checkTrophy(deltaTime)) {
        m_application.lock()->setVisibleScreens({"victory"});
        m_application.lock()->setActiveScreens({"victory"});
        Input::tracker.setCursor(true);
        glfwSetInputMode(WindowHandling::handler.getWindowPointer(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        return;
    }

//    std::cout << "attempting to update animation" << std::endl;
    m_gameWorld->getSystem<DrawAnimationSystem>()->update(deltaTime);
//    std::cout << "updated animation" << std::endl;

    // Phase 0: record initial positions
    m_gameWorld->getSystem<CollisionSystem>()->setInitialPositions();

    // Phase 1: kinetic object movement + kinetic-kinetic collisions
    m_gameWorld->getSystem<SpecialCharacterControllerSystem>()->updateCharacterTransform(deltaTime);

    m_gameWorld->getSystem<EnemySystem>()->updateEnemies(deltaTime);
    m_gameWorld->getSystem<AISystem>()->update(deltaTime);
    m_gameWorld->getSystem<PhysicsSystem>()->applyKinematics(deltaTime);
    m_gameWorld->getSystem<CollisionSystem>()->updateAllCollisions();
    m_gameWorld->getSystem<CollisionSystem>()->resolveCollisions();

    m_gameWorld->getSystem<UniformGridSystem>()->trackMovement();
    m_gameWorld->getSystem<UniformGridSystem>()->updateCells();

    // Phase 2: kinetic-static collision resolution (ellipsoid triangle collisions here)
    m_gameWorld->getSystem<CollisionSystem>()->setFinalPositions();
    m_gameWorld->getSystem<CollisionSystem>()->updateEllipsoidTriangleCollisions();
    m_gameWorld->getSystem<CollisionSystem>()->resolveEllipsoidTriangleCollisions();

    m_gameWorld->getSystem<BulletSystem>()->trackBullets();
    m_gameWorld->getSystem<GunSystem>()->updateGunTransform(deltaTime);
    m_gameWorld->getSystem<GunSystem>()->fireBulletOnMouse(deltaTime);
    m_gameWorld->getSystem<SpecialCharacterControllerSystem>()->checkHitProjectile();

    // Phase 3: move camera (and gun's scope)
    m_gameWorld->getSystem<CameraSystem>()->followCharacter();
    if (Input::tracker.isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT) && !m_gameWorld->getSystem<GunSystem>()->isReloading()) {
        glm::mat4 gunMat = m_gameWorld->getSystem<GunSystem>()->getGunTransform()->getModelMatrix();
        m_gameWorld->getSystem<CameraSystem>()->setCameraTransform(glm::translate(gunMat, glm::vec3(-0.125, 0.25, 0.1)));
    }

    // Phase 4: Cleanup (remove anything that should be deleted)
    m_gameWorld->getSystem<EnemySystem>()->cullEnemies();
    m_gameWorld->getSystem<BulletSystem>()->cullBullets();

    // Misc.
    Global::particleMaster.update(deltaTime);
    m_gameWorld->getSystem<SpawnerSystem>()->attemptToSpawn();
    m_gameWorld->getSystem<CheckpointSystem>()->checkCheckpoints(deltaTime);
    m_gameWorld->getSystem<DoorSystem>()->checkDoors();
    m_gameWorld->getSystem<ShopSystem>()->checkForShops(deltaTime);
    m_gameWorld->getSystem<TrophySystem>()->checkTrophy(deltaTime);
    m_gameWorld->getSystem<JumpPadSystem>()->checkJumpPads();
    m_gameWorld->getSystem<HealthSystem>()->update(deltaTime);

}

void GameScreen::windowResizeEvent(int width, int height) {
    Global::graphics.setWindowSize(glm::ivec2(width, height));
    m_gameWorld->getSystem<CameraSystem>()->resizeCamera(width, height);
}

void GameScreen::keyEvent(int key, int action) {
    if (Input::tracker.isKeyDown(GLFW_KEY_ESCAPE)) {
        m_application.lock()->setVisibleScreens({"game", "overlay", "settings"});
        m_application.lock()->setActiveScreens({"settings"});
        Input::tracker.setCursor(true);
        glfwSetInputMode(WindowHandling::handler.getWindowPointer(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void GameScreen::mousePosEvent(double xpos, double ypos) {

}

void GameScreen::mouseButtonEvent(int button, int action) {

}

void GameScreen::scrollEvent(double distance) {

}

}
