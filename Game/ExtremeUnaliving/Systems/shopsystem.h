#include "Engine/application.h"
#include "Engine/system.h"
#include "glm/glm.hpp"

namespace ExtremeUnaliving {

class ShopSystem : public System
{
public:
    ShopSystem();
    bool checkForShops(float deltaTime);

private:
};

}
