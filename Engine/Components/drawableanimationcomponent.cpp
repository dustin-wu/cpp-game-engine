#include "drawableanimationcomponent.h"
#include "Graphics/Animation/loaders/animatedentityloader.h"
#include "Graphics/Animation/loaders/entityanimationloader.h"


DrawableAnimationComponent::DrawableAnimationComponent(std::shared_ptr<AnimatedModel> entity)
    : entity_(entity),
      localTransform_(std::make_shared<ModelTransform>())
{

}
