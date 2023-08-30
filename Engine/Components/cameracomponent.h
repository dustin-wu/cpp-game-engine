#pragma once

#include "Graphics/camera.h"
#include <Engine/component.h>
#include <memory>

class CameraComponent : public Component
{
public:
    CameraComponent();
    std::shared_ptr<Camera> getCamera();



private:
    std::shared_ptr<Camera> m_camera;
};

