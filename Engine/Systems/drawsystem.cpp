#include "drawsystem.h"
#include "Engine/Systems/camerasystem.h"
#include "Engine/gameobject.h"
#include "Engine/Components/drawablecomponent.h"
#include "Engine/Components/transformcomponent.h"
#include "Graphics/global.h"
#include <glm/gtx/matrix_decompose.hpp>

DrawSystem::DrawSystem()
    : System(typeid(DrawableComponent))
{}

void DrawSystem::draw() {
    Global::graphics.bindShader("particles");
    Global::graphics.setGlobalData(glm::vec3(0.5f));
    Global::graphics.setCameraData(m_gameWorld.lock()->getSystem<CameraSystem>()->getActiveCamera());

    Global::graphics.bindShader("phong");
    Global::graphics.setGlobalData(glm::vec3(0.5f));
    Global::graphics.setCameraData(m_gameWorld.lock()->getSystem<CameraSystem>()->getActiveCamera());

    for (auto& [id, gameObject]: m_gameObjects) {
        std::vector<std::shared_ptr<DrawableComponent>> drawableComponents = gameObject->getComponents<DrawableComponent>();
        std::shared_ptr<TransformComponent> transformComponent = gameObject->getComponent<TransformComponent>();
        std::shared_ptr<ModelTransform> globalTransform = transformComponent->getGlobalTransform();
        glm::mat4 globalMat = globalTransform->getModelMatrix();

        int nComponents = drawableComponents.size();
        for (int i = 0; i < nComponents; i++) {
            std::shared_ptr<DrawableComponent> drawableComponent = drawableComponents[i];
            std::string shape = drawableComponent->getShape();
            std::string material = drawableComponent->getMaterial();
            std::shared_ptr<ModelTransform> localTransform = drawableComponent->getLocalTransform();
            glm::mat4 localMat = localTransform->getModelMatrix();
            glm::mat4 combinedMat = globalMat * localMat;
            std::shared_ptr<ModelTransform> combinedTransform = std::make_shared<ModelTransform>();

            glm::vec3 scale;
            glm::quat rotation;
            glm::vec3 translation;
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(combinedMat, scale, rotation, translation, skew, perspective);
            combinedTransform->translate(translation);
            combinedTransform->rotate(glm::mat4_cast(rotation));
            combinedTransform->scale(scale);

            Global::graphics.drawShape(Global::graphics.getShape(shape), combinedTransform, Global::graphics.getMaterial(material));
        }
    }

}
