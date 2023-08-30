#pragma once

#include "Engine/Systems/initializersystem.h"
#include "Engine/application.h"
#include "Graphics/Animation/animatedModel/animatedmodel.h"

namespace ExtremeUnaliving {

class GameInitializerSystem : public InitializerSystem
{
public:
    GameInitializerSystem();
    void InitializeGame() override;
    void addBullet(std::shared_ptr<ModelTransform> bulletModel);
    void addEnemy(glm::vec3 enemyPos, int enemyType);
    void startGame(bool resetCheckpoints = true);
    void addDoor(glm::vec3 doorPos, GameObjectID lockID, float doorRotation);
    void addCheckpoint(glm::vec3 pos, glm::mat4 rot = glm::mat4(1));
    void addShop(glm::vec3 pos);
    bool nearShop();

private:
    void addPlayer();
    std::vector<glm::vec3> hack;
    std::shared_ptr<AnimatedModel> playerAnimated;
    std::shared_ptr<Animation> playerAnimation;
    std::shared_ptr<AnimatedModel> enemyAAnimated;
    std::shared_ptr<Animation> enemyAHitAnimation;
    std::shared_ptr<Animation> enemyADashAnimation;
    std::shared_ptr<Animation> enemyAAnimation;
    std::shared_ptr<AnimatedModel> enemyBAnimated;
    std::shared_ptr<Animation> enemyBAnimation;
    std::shared_ptr<Animation> enemyBFiringAnimation;
    std::shared_ptr<AnimatedModel> enemyCAnimated;
    std::shared_ptr<Animation> enemyCAnimation;
    std::shared_ptr<Animation> enemyCSmackAnimation;
    std::shared_ptr<Animation> enemyCLeapAnimation;
};


}
