#pragma once

#include "Graphics/modeltransform.h"
#include <Engine/component.h>
#include <memory>
#include <string>

class TransformComponent: public Component
{
public:
    TransformComponent();
    std::shared_ptr<ModelTransform> getGlobalTransform();
    void setDefaultTransform(std::shared_ptr<ModelTransform> defaultTransform);
    void reset() override;


private:
    std::shared_ptr<ModelTransform> m_globalTransform;
    std::shared_ptr<ModelTransform> m_defaultTransform;
};

