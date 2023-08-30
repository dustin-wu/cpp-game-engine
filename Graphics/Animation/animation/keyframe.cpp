#include "keyframe.h"
#include <iostream>

KeyFrame::KeyFrame(float timeStamp, jointmap pose)
    : timeStamp_(timeStamp),
      pose_(pose)
{
//    for (auto const& [jointName, previousJointTransform]: pose) {
//        std::cout << "jointName: " << jointName << std::endl;
//    }
}
