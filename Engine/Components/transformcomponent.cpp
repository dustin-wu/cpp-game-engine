#include "transformcomponent.h"
#include <iostream>

TransformComponent::TransformComponent()
    : m_globalTransform(std::make_shared<ModelTransform>())
    , m_defaultTransform(std::make_shared<ModelTransform>())
{

}

std::shared_ptr<ModelTransform> TransformComponent::getGlobalTransform() {
    return m_globalTransform;
}

void TransformComponent::setDefaultTransform(std::shared_ptr<ModelTransform> defaultTransform) {
    m_defaultTransform = defaultTransform;
    m_globalTransform = std::make_shared<ModelTransform>(*m_defaultTransform);
}

void TransformComponent::reset() {
    m_globalTransform = std::make_shared<ModelTransform>(*m_defaultTransform);
}
