#pragma once

#include "Engine/system.h"
#include "Graphics/modeltransform.h"

class InitializerSystem : public System
{
public:
    InitializerSystem();
    virtual void InitializeGame();
    virtual void InitializePlayer();
    GameObjectID getPlayerID() {return playerID;}
    virtual std::shared_ptr<GameObject> CreateBasePlayerObject(std::shared_ptr<ModelTransform> model);
    virtual std::shared_ptr<GameObject> CreateBaseKineticObject(std::shared_ptr<ModelTransform> model);
    virtual std::shared_ptr<GameObject> CreateBaseStaticObject(std::shared_ptr<ModelTransform> model, std::vector<glm::vec3>& vertices, std::string shape);

protected:
    GameObjectID playerID;
};

