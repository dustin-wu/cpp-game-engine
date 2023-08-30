#pragma once

#include <Engine/component.h>
#include <typeindex>

class CollisionComponent : public Component
{
public:
    CollisionComponent(std::type_index colliderType);
    std::type_index getColliderType();
    virtual ~CollisionComponent() = default;
private:
    std::type_index colliderType_;
};

