#include "camerasystem.h"
#include "Engine/Components/cameracomponent.h"
#include "Engine/Components/playercomponent.h"
#include "Engine/Components/transformcomponent.h"
#include "Engine/Systems/initializersystem.h"
#include <glm/gtx/matrix_decompose.hpp>

CameraSystem::CameraSystem()
    : System(typeid(CameraComponent))
{}

void CameraSystem::setCameraTransform(glm::mat4 model) {
    std::shared_ptr<GameObject> character = m_gameObjects[m_gameWorld.lock()->getPlayerID()];
    std::shared_ptr<CameraComponent> cameraComponent = character->getComponent<CameraComponent>();
    std::shared_ptr<Camera> camera = cameraComponent->getCamera();
    activeCamera = camera;
    float angle;
    glm::vec3 axis;
    glm::vec3 translation, skew, scale;
    glm::quat rotation;
    glm::vec4 perspective;
    glm::decompose(model, scale, rotation, translation, skew, perspective);
    camera->setLook(glm::vec3(0, 0, 1));
    camera->setPos(translation);
    camera->rotate(glm::angle(rotation), glm::axis(rotation));
}

void CameraSystem::followCharacter() {
    std::shared_ptr<GameObject> character = m_gameObjects[m_gameWorld.lock()->getPlayerID()];

    std::shared_ptr<CameraComponent> cameraComponent = character->getComponent<CameraComponent>();
    std::shared_ptr<TransformComponent> transformComponent = character->getComponent<TransformComponent>();
    std::shared_ptr<PlayerComponent> playerComponent = character->getComponent<PlayerComponent>();

    std::shared_ptr<Camera> camera = cameraComponent->getCamera();
    activeCamera = camera;

    float eyeHeight = playerComponent->getEyeHeight();
    float headTurn = playerComponent->getHeadTurn();
    float headTilt = playerComponent->getHeadTilt();

    camera->setLook(glm::vec3(0, 0, 1));
    camera->setPos(glm::vec3(0, 0, 0));
    camera->rotate(-headTurn, glm::vec3(0, 1, 0));
    camera->rotate(-headTilt, glm::cross(camera->getLook(), glm::vec3(0, 1, 0)));

    glm::vec3 lookVec = glm::normalize(camera->getLook());
    glm::vec3 firstPersonOffset = glm::vec3(0, 1.5, 0) + 0.2f * lookVec;
    camera->setPos(transformComponent->getGlobalTransform()->getPos() + firstPersonOffset);
    glm::vec3 thirdPersonOffset = lookVec * -2.f + 0.5f * glm::normalize(glm::cross(lookVec, firstPersonOffset));
    // glm::vec3 cameraTranslate = (1 - playerComponent->getThirdPersonFactor()) * firstPersonOffset + playerComponent->getThirdPersonFactor() * thirdPersonOffset;
    glm::vec3 cameraTranslate = playerComponent->getThirdPersonFactor() * thirdPersonOffset;
    camera->translate(cameraTranslate);
}

void CameraSystem::resizeCamera(int width, int height) {
    std::shared_ptr<GameObject> character = m_gameObjects[m_gameObjects.begin()->first];
    std::shared_ptr<CameraComponent> cameraComponent = character->getComponent<CameraComponent>();
    std::shared_ptr<Camera> camera = cameraComponent->getCamera();
    camera->resize(width, height);
}

std::shared_ptr<Camera> CameraSystem::getActiveCamera() {
    return activeCamera;
}
