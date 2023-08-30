#pragma once

#include <string>
#include <Engine/component.h>
#include "Graphics/modeltransform.h"

class DrawableComponent : public Component
{
public:
    DrawableComponent();
    void setShape(std::string shape);
    void setMaterial(std::string material);
    std::string getShape();
    std::string getMaterial();
    std::shared_ptr<ModelTransform> getLocalTransform();


private:
    std::string shape_;
    std::string material_;
    std::shared_ptr<ModelTransform> m_localTransform;
};

