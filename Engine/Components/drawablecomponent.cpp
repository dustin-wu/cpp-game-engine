#include "drawablecomponent.h"


DrawableComponent::DrawableComponent()
    : m_localTransform(std::make_shared<ModelTransform>())
{

}

void DrawableComponent::setShape(std::string shape) {
    shape_ = shape;
}

void DrawableComponent::setMaterial(std::string material) {
    material_ = material;
}

std::string DrawableComponent::getShape() {
    return shape_;
}

std::string DrawableComponent::getMaterial() {
    return material_;
}

std::shared_ptr<ModelTransform> DrawableComponent::getLocalTransform() {
    return m_localTransform;
}
