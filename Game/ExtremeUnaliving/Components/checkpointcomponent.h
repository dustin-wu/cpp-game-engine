
#include "Engine/component.h"
#include "glm/glm.hpp"

namespace ExtremeUnaliving {

class CheckpointComponent : public Component
{
public:
    CheckpointComponent(glm::vec3 position, glm::mat4 rotation);
    glm::vec3 getPosition() { return position_;}
    glm::mat4 getRotation() { return rotation_;}

private:
    glm::vec3 position_;
    glm::mat4 rotation_;
};

}
