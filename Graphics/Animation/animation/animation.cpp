#include "animation.h"

Animation::Animation(float lengthInSeconds, std::vector<std::shared_ptr<KeyFrame>> keyFrames)
    : length_(lengthInSeconds),
      keyFrames_(keyFrames)
{

}
