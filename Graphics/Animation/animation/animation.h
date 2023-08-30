#pragma once

#include <vector>
#include "keyframe.h"

class Animation
{
public:
    Animation(float lengthInSeconds, std::vector<std::shared_ptr<KeyFrame>> keyFrames);

    float getLength(){ return length_; }
    std::vector<std::shared_ptr<KeyFrame>> getKeyFrames(){ return keyFrames_; }

private:
    const float length_;
    std::vector<std::shared_ptr<KeyFrame>> keyFrames_;

};

