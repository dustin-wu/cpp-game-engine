#include "bulletcomponent.h"

namespace ExtremeUnaliving {

BulletComponent::BulletComponent()
    : bulletSpeed_(40)
{

}

float BulletComponent::getSpeed() {
    return bulletSpeed_;
}


}
