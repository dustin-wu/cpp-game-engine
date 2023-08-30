#include "bulletcomponent.h"

BulletComponent::BulletComponent()
    : bulletSpeed_(20)
{

}

float BulletComponent::getSpeed() {
    return bulletSpeed_;
}
