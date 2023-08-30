#pragma once


#include "Engine/component.h"
#include "Graphics/Animation/animatedModel/animatedmodel.h"
#include "Graphics/modeltransform.h"
#include <string>

class DrawableAnimationComponent : public Component
{
public:
    DrawableAnimationComponent(std::shared_ptr<AnimatedModel> entity);
    std::shared_ptr<AnimatedModel> getEntity(){ return entity_; }
    void setAnimation(std::shared_ptr<Animation> animation) { animation_ = animation; entity_->doAnimation(animation_); }
    std::shared_ptr<ModelTransform> getLocalTransform() { return localTransform_; }

private:
    std::shared_ptr<AnimatedModel> entity_;
    std::shared_ptr<Animation> animation_;
    std::shared_ptr<ModelTransform> localTransform_;
};

