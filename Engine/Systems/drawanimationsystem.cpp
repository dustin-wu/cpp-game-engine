#include "drawanimationsystem.h"
#include "Engine/Components/drawableanimationcomponent.h"
#include "Engine/Components/transformcomponent.h"
#include "Engine/Systems/camerasystem.h"
#include "Graphics/global.h"

DrawAnimationSystem::DrawAnimationSystem()
    : System(typeid(DrawableAnimationComponent))
{}

void DrawAnimationSystem::draw() {
    Global::graphics.bindShader("animation");
    Global::graphics.setGlobalData(glm::vec3(0.5f));
    Global::graphics.setCameraData(m_gameWorld.lock()->getSystem<CameraSystem>()->getActiveCamera());

    for (auto& [id, gameObject]: m_gameObjects) {
        std::shared_ptr<DrawableAnimationComponent> drawableComponent = gameObject->getComponent<DrawableAnimationComponent>();
        std::shared_ptr<TransformComponent> transformComponent = gameObject->getComponent<TransformComponent>();
        std::shared_ptr<ModelTransform> globalTransform = transformComponent->getGlobalTransform();
        glm::mat4 globalMat = globalTransform->getModelMatrix();
        glm::mat4 localMat = drawableComponent->getLocalTransform()->getModelMatrix();
        glm::mat4 combinedMat = globalMat * localMat;
        Global::graphics.bindShader("animation");
        Global::graphics.drawAnimatedEntity(drawableComponent->getEntity(), combinedMat);

    }
}

void DrawAnimationSystem::update(float deltaTime) {
    for (auto& [id, gameObject]: m_gameObjects) {
        gameObject->getComponent<DrawableAnimationComponent>()->getEntity()->update(deltaTime);
    }
}

