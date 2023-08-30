#pragma once

#include "Engine/system.h"
#include "Graphics/camera.h"
#include "Graphics/modeltransform.h"

class CameraSystem : public System
{
public:
    CameraSystem();

    void followCharacter();
    void setCameraTransform(glm::mat4);
    void resizeCamera(int width, int height);
    std::shared_ptr<Camera> getActiveCamera();

private:
    std::shared_ptr<Camera> activeCamera;
};
