#pragma once

#include <string>
#include <unordered_map>
#include "jointtransform.h"

typedef std::unordered_map<std::string, std::shared_ptr<JointTransform>> jointmap;

class KeyFrame
{
public:
    KeyFrame(float timeStamp, jointmap pose);
    float getTimeStamp(){ return timeStamp_; }
    jointmap getPose(){ return pose_; }


private:
    const float timeStamp_;
    jointmap pose_;
};

