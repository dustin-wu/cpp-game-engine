#include "cameracomponent.h"

CameraComponent::CameraComponent()
    : m_camera(std::make_shared<Camera>())
{

}

std::shared_ptr<Camera> CameraComponent::getCamera() {
    return m_camera;
}
