#include "gameinitializersystem.h"
#include "Engine/Components/BehaviorTree/selector.h"
#include "Engine/Components/BehaviorTree/sequence.h"
#include "Engine/Components/CollisionComponents/cylindercollisioncomponent.h"
#include "Engine/Components/CollisionComponents/ellipsoidcollisioncomponent.h"
#include "Engine/Components/CollisionComponents/trianglecollisioncomponent.h"
#include "Engine/Components/aicomponent.h"
#include "Engine/Components/drawableanimationcomponent.h"
#include "Engine/Components/drawablecomponent.h"
#include "Engine/Components/navmeshcomponent.h"
#include "Engine/Components/playercomponent.h"
#include "Engine/Components/rigidbodycomponent.h"
#include "Engine/Components/transformcomponent.h"
#include "Engine/Systems/collisionsystem.h"
#include "Engine/Systems/drawanimationsystem.h"
#include "Engine/Systems/navmeshsystem.h"
#include "Game/ExtremeUnaliving/BehaviorTree/actionhit.h"
#include "Game/ExtremeUnaliving/BehaviorTree/actionleap.h"
#include "Game/ExtremeUnaliving/BehaviorTree/actionlungeatplayer.h"
#include "Game/ExtremeUnaliving/BehaviorTree/actionpursueplayer.h"
#include "Game/ExtremeUnaliving/BehaviorTree/actionrunaway.h"
#include "Game/ExtremeUnaliving/BehaviorTree/actionshootplayer.h"
#include "Game/ExtremeUnaliving/BehaviorTree/actionsmack.h"
#include "Game/ExtremeUnaliving/BehaviorTree/conditionhealthislow.h"
#include "Game/ExtremeUnaliving/BehaviorTree/conditionplayerisclose.h"
#include "Game/ExtremeUnaliving/BehaviorTree/conditionplayerisfar.h"
#include "Game/ExtremeUnaliving/Components/bulletcomponent.h"
#include "Game/ExtremeUnaliving/Components/doorcomponent.h"
#include "Game/ExtremeUnaliving/Components/enemycomponent.h"
#include "Game/ExtremeUnaliving/Components/guncomponent.h"
#include "Game/ExtremeUnaliving/Components/jumppadcomponent.h"
#include "Game/ExtremeUnaliving/Components/shopcomponent.h"
#include "Game/ExtremeUnaliving/Components/trophycomponent.h"
#include "Game/ExtremeUnaliving/ShopHandler.h"
#include "Game/ExtremeUnaliving/Systems/bulletsystem.h"
#include "Game/ExtremeUnaliving/Systems/checkpointsystem.h"
#include "Game/ExtremeUnaliving/Systems/shopsystem.h"
#include "Game/ExtremeUnaliving/Systems/doorsystem.h"
#include "Game/ExtremeUnaliving/Systems/enemysystem.h"
#include "Game/ExtremeUnaliving/Systems/gunsystem.h"
#include "Game/ExtremeUnaliving/Systems/healthsystem.h"
#include "Game/ExtremeUnaliving/Systems/jumppadsystem.h"
#include "Game/ExtremeUnaliving/Systems/spawnersystem.h"
#include "Game/ExtremeUnaliving/Systems/specialcharactercontrollersystem.h"
#include "Game/ExtremeUnaliving/Systems/trophysystem.h"
#include "Graphics/global.h"
#include "Graphics/Animation/loaders/animatedentityloader.h"
#include "Graphics/Animation/loaders/entityanimationloader.h"

namespace ExtremeUnaliving {

GameInitializerSystem::GameInitializerSystem()
{

}

void GameInitializerSystem::startGame(bool resetCheckpoints) {
    if (resetCheckpoints) { // if hard reset via going back to title
        m_gameWorld.lock()->getSystem<CheckpointSystem>()->resetCheckpoints();
        m_gameWorld.lock()->getSystem<DoorSystem>()->resetDoors();
        ShopHandling::handler.reset();
    }

    std::shared_ptr<GameObject> playerObject = m_gameWorld.lock()->getGameObject(playerID);
    glm::vec3 checkpointPos = m_gameWorld.lock()->getSystem<CheckpointSystem>()->getLatestCheckpointPos();
    glm::mat4 checkpointRot = m_gameWorld.lock()->getSystem<CheckpointSystem>()->getLatestCheckpointRot();
    playerObject->getComponent<TransformComponent>()->getGlobalTransform()->setPos(checkpointPos + glm::vec3(0, 1, 0));
    playerObject->getComponent<TransformComponent>()->getGlobalTransform()->setRotation(checkpointRot);
    playerObject->getComponent<PlayerComponent>()->setHeadTurn(glm::acos(checkpointRot[0][0]));
}

bool GameInitializerSystem::nearShop() {
    return m_gameWorld.lock()->getSystem<ShopSystem>()->checkForShops(0);
}

void GameInitializerSystem::InitializeGame() {
    std::cout << "initializing game" <<std::endl;
    playerAnimated = AnimatedEntityLoader::loadEntity("Resources/Animations/test8.dae", "cowboyTexture");
    playerAnimation = EntityAnimationLoader::loadAnimation("Resources/Animations/playerrunning.dae");
    enemyAAnimated = AnimatedEntityLoader::loadEntity("Resources/Animations/robotArunning.dae", "robotTexture");
    enemyAAnimation = EntityAnimationLoader::loadAnimation("Resources/Animations/robotArunning.dae");
    enemyAHitAnimation = EntityAnimationLoader::loadAnimation("Resources/Animations/robotAhit.dae");
    enemyADashAnimation = EntityAnimationLoader::loadAnimation("Resources/Animations/robotAdash.dae");

    enemyBAnimated = AnimatedEntityLoader::loadEntity("Resources/Animations/robotBrunning.dae", "robotTexture");
    enemyBAnimation = EntityAnimationLoader::loadAnimation("Resources/Animations/robotBrunning.dae");
    enemyBFiringAnimation = EntityAnimationLoader::loadAnimation("Resources/Animations/robotBshooting.dae");
    enemyCAnimated = AnimatedEntityLoader::loadEntity("Resources/Animations/robotCrunning.dae", "robotTexture");
    enemyCAnimation = EntityAnimationLoader::loadAnimation("Resources/Animations/robotCrunning.dae");
    enemyCSmackAnimation = EntityAnimationLoader::loadAnimation("Resources/Animations/robotCsmack.dae");
    enemyCLeapAnimation = EntityAnimationLoader::loadAnimation("Resources/Animations/robotCleap.dae");


    std::vector<glm::vec3> testMeshVertices = Global::graphics.addShape("testMesh", "Resources/Meshes/ceiling.obj");
    std::vector<glm::vec3> rampMeshVertices = Global::graphics.addShape("rampMesh", "Resources/Meshes/ramp.obj");
    std::vector<glm::vec3> stairsMeshVertices = Global::graphics.addShape("stairsMesh", "Resources/Meshes/stairs.obj");
    std::vector<glm::vec3> jumpPadMeshVertices = Global::graphics.addShape("jumppadMesh", "Resources/Meshes/jumppad.obj");
    std::vector<glm::vec3> gunVertices = Global::graphics.addShape("gunMesh", "Resources/Meshes/revolver.obj");
    std::vector<glm::vec3> zombieVertices = Global::graphics.addShape("zombieMesh", "Resources/Meshes/zombie.obj");
//    std::vector<glm::vec3> envMeshVertices = Global::graphics.addShape("envMesh", "Resources/Meshes/level1.obj");
    std::vector<glm::vec3> level1VerticesFloor = Global::graphics.addShape("floorMesh", "Resources/Meshes/level1floors.obj");
    std::vector<glm::vec3> level1VerticesWalls = Global::graphics.addShape("wallMesh", "Resources/Meshes/level1walls.obj");
    std::vector<glm::vec3> level1VerticesInnerWalls = Global::graphics.addShape("innerWallMesh", "Resources/Meshes/level1innerwalls.obj");
    std::vector<glm::vec3> doorMeshVertices = Global::graphics.addShape("doorMesh", "Resources/Meshes/door.obj");

    std::vector<glm::vec3> finalVerticesFloor = Global::graphics.addShape("finalFloor", "Resources/Meshes/enviornmentFinalFloors.obj");
    std::vector<glm::vec3> finalVerticesWalls1 = Global::graphics.addShape("finalWalls1", "Resources/Meshes/enviornmentFinalWalls1.obj");
    std::vector<glm::vec3> finalVerticesWalls2 = Global::graphics.addShape("finalWalls2", "Resources/Meshes/enviornmentFinalWalls2.obj");
    std::vector<glm::vec3> finalVerticesCeilings1 = Global::graphics.addShape("finalCeilings1", "Resources/Meshes/enviornmentFinalCeilings1.obj");
    std::vector<glm::vec3> finalVerticesCeilings2 = Global::graphics.addShape("finalCeilings2", "Resources/Meshes/enviornmentFinalCeilings2.obj");

    Global::graphics.addShape("sprayMesh", "Resources/Meshes/spray.obj");
    Global::graphics.addShape("trophyMesh", "Resources/Meshes/trophy.obj");


    hack = jumpPadMeshVertices;

    Global::graphics.addMaterial("cowboyTexture", "Resources/Images/cowboy.png");
    Global::graphics.addMaterial("ammoTexture", "Resources/Images/ammo.png");
    Global::graphics.addMaterial("bulletsparks", "Resources/Images/bulletsparks.png");
    Global::graphics.addMaterial("bloodsparks", "Resources/Images/bloodsparks.png");
    Global::graphics.addMaterial("staminaparticle", "Resources/Images/staminaparticle.png");
    Global::graphics.addMaterial("title", "Resources/Images/title.png");
    Global::graphics.addMaterial("defeat", "Resources/Images/defeat.png");
    Global::graphics.addMaterial("star", "Resources/Images/star.png");

    Global::graphics.addMaterial("item1", "Resources/Images/item1.png");
    Global::graphics.addMaterial("item2", "Resources/Images/item2.png");
    Global::graphics.addMaterial("item3", "Resources/Images/item3.png");
    Global::graphics.addMaterial("item4", "Resources/Images/item4.png");
    Global::graphics.addMaterial("item5", "Resources/Images/item5.png");
    Global::graphics.addMaterial("item6", "Resources/Images/item6.png");
    Global::graphics.addMaterial("steel", "Resources/Images/steel.png");
    Global::graphics.addMaterial("tutorial", "Resources/Images/tutorial.png");

    Global::graphics.addMaterial("white", "Resources/Images/white.png");
    Global::graphics.addMaterial("beamgradient", "Resources/Images/beamgradient.png");

    Global::graphics.addMaterial("gunTexture", "Resources/Images/gun.jpg");
    Global::graphics.addMaterial("bulletTexture", "Resources/Images/bullet.jpg");
    Global::graphics.addMaterial("grassTexture", "Resources/Images/grass.png");
    Global::graphics.addMaterial("zombieTexture", "Resources/Images/zombie.jpg");
    Global::graphics.addMaterial("sandTexture", "Resources/Images/sand2.jpeg");
    Global::graphics.addMaterial("wallTexture", "Resources/Images/BRICK_4C.png");
    Global::graphics.addMaterial("jumpTexture", "Resources/Images/jump.jpg");
    Global::graphics.addMaterial("crateTexture", "Resources/Images/TECH_0F.png");
    Global::graphics.addMaterial("buttonTexture", "Resources/Images/CRATE_1L.png");

    Global::graphics.addMaterial("wall1Texture", "Resources/Images/LAB_1A.png");
    Global::graphics.addMaterial("wall2Texture", "Resources/Images/SUPPORT_5A.png");
    Global::graphics.addMaterial("ceiling1Texture", "Resources/Images/PIPES_2A.png");
    Global::graphics.addMaterial("ceiling2Texture", "Resources/Images/LIGHT_1B.png");
    Global::graphics.addMaterial("floorTexture", "Resources/Images/FLOOR_1A.png");

    Global::graphics.addMaterial("healthbarTexture", "Resources/Images/healthbarpixel.png");
    Global::graphics.addMaterial("staminaTexture", "Resources/Images/stamina.png");
    Global::graphics.addMaterial("barbackgroundTexture", "Resources/Images/barbackground.png");
    Global::graphics.addMaterial("robotTexture", "Resources/Images/robot.png");
    Global::graphics.addMaterial("checkpointTexture", "Resources/Images/checkpoint.png");
    Global::graphics.addMaterial("doorTexture", "Resources/Images/DOOR_2E.png");
    Global::graphics.addMaterial("shopTexture", "Resources/Images/shop.png");


    std::shared_ptr<GameWorld> gameWorld = m_gameWorld.lock();

    InitializerSystem::InitializeGame();
    gameWorld->addSystem<SpecialCharacterControllerSystem>();
    gameWorld->addSystem<JumpPadSystem>();
    gameWorld->addSystem<GunSystem>();
    gameWorld->addSystem<BulletSystem>();
    gameWorld->addSystem<EnemySystem>();
    gameWorld->addSystem<SpecialCharacterControllerSystem>();
    gameWorld->addSystem<JumpPadSystem>();
    gameWorld->addSystem<HealthSystem>();
    gameWorld->addSystem<DrawAnimationSystem>();
    gameWorld->addSystem<SpawnerSystem>();
    gameWorld->addSystem<CheckpointSystem>();
    gameWorld->addSystem<DoorSystem>();
    gameWorld->addSystem<ShopSystem>();
    gameWorld->addSystem<TrophySystem>();

    InitializerSystem::InitializePlayer();
    addPlayer();

    std::queue<int> spawnCounts;
    spawnCounts.push(1);
    std::queue<float> spawnCooldowns;
    spawnCooldowns.push(5);

    std::vector<std::shared_ptr<Light>> lights;
    std::shared_ptr<Light> light = std::make_shared<Light>(LightType::DIRECTIONAL, glm::vec3(1, -1, 1));
    lights.push_back(light);
    Global::graphics.setLights(lights);


    std::shared_ptr<ModelTransform> jumpPadTransform = std::make_shared<ModelTransform>();
    jumpPadTransform->setPos(glm::vec3(-66, 0.11, -79));
    jumpPadTransform->setRotation(-M_PI/2, glm::vec3(0, 1, 0));
    jumpPadTransform->setScale(glm::vec3(1, 1, 1));
    std::shared_ptr<GameObject> jumpPadObj = CreateBaseStaticObject(jumpPadTransform, jumpPadMeshVertices, "jumppadMesh");
    std::shared_ptr<JumpPadComponent> jumpPadComponent = std::make_shared<JumpPadComponent>();
    jumpPadComponent->setJumpPadForce(17.f);
    jumpPadObj->addComponent<JumpPadComponent>(jumpPadComponent);
    jumpPadObj->getComponent<DrawableComponent>()->setMaterial("jumpTexture");
    gameWorld->addGameObject(jumpPadObj);

    std::shared_ptr<ModelTransform> jumpPad2Transform = std::make_shared<ModelTransform>();
    jumpPad2Transform->setPos(glm::vec3(-27, 13.35, -89));
    jumpPad2Transform->setRotation(-M_PI/2, glm::vec3(0, 1, 0));
    jumpPad2Transform->setScale(glm::vec3(1, 1, 1));
    std::shared_ptr<GameObject> jumpPad2Obj = CreateBaseStaticObject(jumpPad2Transform, jumpPadMeshVertices, "jumppadMesh");
    std::shared_ptr<JumpPadComponent> jumpPad2Component = std::make_shared<JumpPadComponent>();
    jumpPad2Component->setJumpPadForce(13.f);
    jumpPad2Obj->addComponent<JumpPadComponent>(jumpPad2Component);
    jumpPad2Obj->getComponent<DrawableComponent>()->setMaterial("jumpTexture");
    gameWorld->addGameObject(jumpPad2Obj);

    std::shared_ptr<ModelTransform> floorObjTransform = std::make_shared<ModelTransform>();
    floorObjTransform->setScale(glm::vec3(1, 1, 1));
    floorObjTransform->setPos(glm::vec3(0, 0.1, 0));
    std::shared_ptr<GameObject> floorObj = CreateBaseStaticObject(floorObjTransform, finalVerticesFloor, "finalFloor");
    floorObj->getComponent<DrawableComponent>()->setMaterial("floorTexture");
    gameWorld->addGameObject(floorObj);
    std::shared_ptr<ModelTransform> wall1ObjTransform = std::make_shared<ModelTransform>();
    wall1ObjTransform->setScale(glm::vec3(1, 1, 1));
    wall1ObjTransform->setPos(glm::vec3(0, 0.1, 0));
    std::shared_ptr<GameObject> wall1Obj = CreateBaseStaticObject(wall1ObjTransform, finalVerticesWalls1, "finalWalls1");
    wall1Obj->getComponent<DrawableComponent>()->setMaterial("wall1Texture");
    gameWorld->addGameObject(wall1Obj);
    std::shared_ptr<ModelTransform> wall2ObjTransform = std::make_shared<ModelTransform>();
    wall2ObjTransform->setScale(glm::vec3(1, 1, 1));
    wall2ObjTransform->setPos(glm::vec3(0, 0.1, 0));
    std::shared_ptr<GameObject> wall2Obj = CreateBaseStaticObject(wall2ObjTransform, finalVerticesWalls2, "finalWalls2");
    wall2Obj->getComponent<DrawableComponent>()->setMaterial("wall2Texture");
    gameWorld->addGameObject(wall2Obj);
    std::shared_ptr<ModelTransform> ceiling1ObjTransform = std::make_shared<ModelTransform>();
    ceiling1ObjTransform->setScale(glm::vec3(1, 1, 1));
    ceiling1ObjTransform->setPos(glm::vec3(0, 0.1, 0));
    std::shared_ptr<GameObject> ceiling1Obj = CreateBaseStaticObject(ceiling1ObjTransform, finalVerticesCeilings1, "finalCeilings1");
    ceiling1Obj->getComponent<DrawableComponent>()->setMaterial("ceiling1Texture");
    gameWorld->addGameObject(ceiling1Obj);
    std::shared_ptr<ModelTransform> ceiling2ObjTransform = std::make_shared<ModelTransform>();
    ceiling2ObjTransform->setScale(glm::vec3(1, 1, 1));
    ceiling2ObjTransform->setPos(glm::vec3(0, 0.1, 0));
    std::shared_ptr<GameObject> ceiling2Obj = CreateBaseStaticObject(ceiling2ObjTransform, finalVerticesCeilings2, "finalCeilings2");
    ceiling2Obj->getComponent<DrawableComponent>()->setMaterial("ceiling2Texture");
    gameWorld->addGameObject(ceiling2Obj);

    std::pair<std::vector<glm::vec3>, std::vector<glm::ivec3>> navmeshData = Global::graphics.getNavmeshData("Resources/Meshes/enviornmentFinalNav.obj");
    std::vector<glm::vec3> navmeshVertices = navmeshData.first;
    std::vector<glm::vec3> navmeshVerticesTransformed;
    for (glm::vec3 vertex: navmeshVertices) {
        navmeshVerticesTransformed.push_back(floorObjTransform->getModelMatrix() * glm::vec4(vertex, 1.0f));
    }
    std::vector<glm::ivec3> navmeshFaces = navmeshData.second;
    std::shared_ptr<GameObject> navmeshObj = std::make_shared<GameObject>();
    std::shared_ptr<NavmeshComponent> navmeshComponent = std::make_shared<NavmeshComponent>(navmeshVerticesTransformed, navmeshFaces);
    navmeshObj->addComponent<NavmeshComponent>(navmeshComponent);
    gameWorld->addGameObject(navmeshObj);

    std::shared_ptr<GameObject> spawner1 = std::make_shared<GameObject>();
    std::shared_ptr<SpawnerComponent> spawner1Config = std::make_shared<SpawnerComponent>(glm::vec3(-19.0229, 0.101, -0.336797));
    spawner1Config->insertWave({SpawnInfo{glm::vec3(-28.0034, 0.101, 6.01944), 0},
                                SpawnInfo{glm::vec3(-34.407, 0.11, -5.33211), 0}
                               });
    spawner1->addComponent<SpawnerComponent>(spawner1Config);
    gameWorld->addGameObject(spawner1);

    std::shared_ptr<GameObject> spawner2 = std::make_shared<GameObject>();
    std::shared_ptr<SpawnerComponent> spawner2Config = std::make_shared<SpawnerComponent>(glm::vec3(-65.9944, 0.100238, -29.9603));
    spawner2Config->insertWave({SpawnInfo{glm::vec3(-73.5751, 0.101, -55.1875), 0},
                                SpawnInfo{glm::vec3(-58.0831, 0.101, -55.0892), 0},
                                SpawnInfo{glm::vec3(-58.0477, 0.101, -48.3419), 0},
                                SpawnInfo{glm::vec3(-71.6383, 0.101, -48.5309), 0},
                               });
    spawner2Config->insertWave({SpawnInfo{glm::vec3(-73.5751, 0.101, -55.1875), 0},
                                SpawnInfo{glm::vec3(-58.0831, 0.101, -55.0892), 0},
                                SpawnInfo{glm::vec3(-58.0477, 0.101, -48.3419), 0},
                                SpawnInfo{glm::vec3(-71.6383, 0.101, -48.5309), 0},
                                SpawnInfo{glm::vec3(-74.4937, 0.10041, -42.7485), 0},
                               });
    spawner2->addComponent<SpawnerComponent>(spawner2Config);
    gameWorld->addGameObject(spawner2);

    std::shared_ptr<GameObject> spawner3 = std::make_shared<GameObject>();
    std::shared_ptr<SpawnerComponent> spawner3Config = std::make_shared<SpawnerComponent>(glm::vec3(-38.4353, 13.2544, -87.5222));
    spawner3Config->insertWave({SpawnInfo{glm::vec3(-25.6043, 13.2541, -96.2596), 0},
                                SpawnInfo{glm::vec3(-26.2598, 13.2538, -82.6077), 0},
                                SpawnInfo{glm::vec3(-19.1017, 20.8716, -95.5414), 1},
                                SpawnInfo{glm::vec3(-19.256, 20.8707, -90.1061), 1},
                                SpawnInfo{glm::vec3(-19.2499, 20.8713, -82.7971), 1}
                               });

    spawner3Config->insertWave({SpawnInfo{glm::vec3(-2.67436, 20.8716, -80.6169), 0},
                                SpawnInfo{glm::vec3(-3.17227, 20.8713, -86.4178), 0},
                                SpawnInfo{glm::vec3(-2.20826, 20.8707, -95.0702), 0},
                                SpawnInfo{glm::vec3(-19.1017, 20.8716, -95.5414), 1},
                                SpawnInfo{glm::vec3(-19.256, 20.8707, -90.1061), 1},
                                SpawnInfo{glm::vec3(-19.2499, 20.8713, -82.7971), 1}
                               });
    spawner3->addComponent<SpawnerComponent>(spawner3Config);
    gameWorld->addGameObject(spawner3);

    std::shared_ptr<GameObject> spawner4 = std::make_shared<GameObject>();
    std::shared_ptr<SpawnerComponent> spawner4Config = std::make_shared<SpawnerComponent>(glm::vec3(6.52518, 0.100162, -53.8565));
    spawner4Config->insertWave({SpawnInfo{glm::vec3(13.4169, 0.101, -36.0374), 0},
                                SpawnInfo{glm::vec3(9.92469, 0.101, -35.4872), 0},
                                SpawnInfo{glm::vec3(5.16994, 0.101, -35.5396), 0},
                                SpawnInfo{glm::vec3(-1.8706, 0.101, -35.45), 0},
                                SpawnInfo{glm::vec3(13.2346, 0.101, -20.8268), 1},
                                SpawnInfo{glm::vec3(7.5245, 0.101, -20.7008), 1},
                                SpawnInfo{glm::vec3(0.0650727, 0.101, -20.6133), 1}
                               });
    spawner4Config->insertWave({SpawnInfo{glm::vec3(-6.07164, 13.5465, -15.9295), 0},
                                SpawnInfo{glm::vec3(-0.864129, 13.5459, -15.7807), 0},
                                SpawnInfo{glm::vec3(3.29985, 13.5465, -15.6807), 0},
                                SpawnInfo{glm::vec3(8.01161, 13.5465, -15.5677), 0},
                                SpawnInfo{glm::vec3(16.3369, 13.5465, -15.4141), 0},
                                SpawnInfo{glm::vec3(10.4661, 13.5465, -18.7924), 1},
                                SpawnInfo{glm::vec3(16.1144, 13.5459, -18.8481), 1},
                                SpawnInfo{glm::vec3(-1.11524, 13.5465, -18.7141), 1}
                               });
    spawner4Config->insertWave({SpawnInfo{glm::vec3(-0.0445485, 0.101, -51.933), 1},
                                SpawnInfo{glm::vec3(-4.70549, 0.101, -51.8611), 1},
                                SpawnInfo{glm::vec3(12.8477, 0.101, -50.6512), 1},
                                SpawnInfo{glm::vec3(18.7313, 0.101, -51.3154), 1},
                                SpawnInfo{glm::vec3(10.4661, 13.5465, -18.7924), 1},
                                SpawnInfo{glm::vec3(16.1144, 13.5459, -18.8481), 1},
                                SpawnInfo{glm::vec3(-1.11524, 13.5465, -18.7141), 1}
                               });
    spawner4->addComponent<SpawnerComponent>(spawner4Config);
    gameWorld->addGameObject(spawner4);

    std::shared_ptr<GameObject> spawner5 = std::make_shared<GameObject>();
    std::shared_ptr<SpawnerComponent> spawner5Config = std::make_shared<SpawnerComponent>(glm::vec3(13.3897, 0.101, 16.2039));
    spawner5Config->insertWave({SpawnInfo{glm::vec3(45.2, 0.101, 17.2237), 0},
                                SpawnInfo{glm::vec3(45.1888, 0.100199, 11.6824), 0},
                                SpawnInfo{glm::vec3(45.1429, 0.101, 6.04431), 0},
                                SpawnInfo{glm::vec3(45.2897, 0.101, 1.60604), 0},
                                SpawnInfo{glm::vec3(45.3549, 0.101, -2.08293), 0},
                                SpawnInfo{glm::vec3(45.2935, 0.101, -5.54362), 0},
                                SpawnInfo{glm::vec3(59.9759, 0.101, 14.6795), 1},
                                SpawnInfo{glm::vec3(60.535, 0.101, 4.80085), 1},
                                SpawnInfo{glm::vec3(60.859, 0.101, -2.08351), 1}
                               });
    spawner5Config->insertWave({SpawnInfo{glm::vec3(48.4635, 0.101, 5.06267), 2}
                               });
    spawner5Config->insertWave({SpawnInfo{glm::vec3(40.033, 0.101, 14.3741), 2},
                                SpawnInfo{glm::vec3(56.4491, 0.101, -2.41286), 2}
                               });
    spawner5Config->insertWave({SpawnInfo{glm::vec3(35.4074, 0.101, -4.69324), 2},
                                SpawnInfo{glm::vec3(59.6665, 0.100182, -3.72476), 2},
                                SpawnInfo{glm::vec3(59.029, 0.101, 14.3322), 2},
                                SpawnInfo{glm::vec3(39.5455, 0.101, 13.1018), 2}
                               });
    spawner5->addComponent<SpawnerComponent>(spawner5Config);
    gameWorld->addGameObject(spawner5);

    std::shared_ptr<GameObject> spawner6 = std::make_shared<GameObject>();
    std::shared_ptr<SpawnerComponent> spawner6Config = std::make_shared<SpawnerComponent>(glm::vec3(47.3899, 0.101, -82.5693));
    spawner6Config->insertWave({SpawnInfo{glm::vec3(78.5634, 13.5452, -128.659), 1},
                                SpawnInfo{glm::vec3(78.9564, 13.5452, -113.252), 1},
                                SpawnInfo{glm::vec3(79.0263, 13.5452, -104.222), 1},
                                SpawnInfo{glm::vec3(78.7498, 13.5452, -90.4555), 1},
                                SpawnInfo{glm::vec3(18.6634, 13.5443, -89.0945), 1},
                                SpawnInfo{glm::vec3(18.9138, 13.5443, -100.191), 1},
                                SpawnInfo{glm::vec3(18.6658, 13.5452, -113.078), 1},
                                SpawnInfo{glm::vec3(18.4099, 13.5452, -126.178), 1},
                                SpawnInfo{glm::vec3(57.3392, 0.100052, -120.794), 0},
                                SpawnInfo{glm::vec3(51.266, 0.101, -120.457), 0},
                                SpawnInfo{glm::vec3(44.6588, 0.101, -120.532), 0},
                                SpawnInfo{glm::vec3(33.2848, 0.10038, -120.613), 0},
                                SpawnInfo{glm::vec3(33.2588, 0.101, -115.622), 0},
                                SpawnInfo{glm::vec3(41.623, 0.101, -115.515), 0},
                                SpawnInfo{glm::vec3(52.4713, 0.101, -115.662), 0},
                                SpawnInfo{glm::vec3(59.8351, 0.100893, -115.289), 0}
                               });
    spawner6Config->insertWave({SpawnInfo{glm::vec3(78.5634, 13.5452, -128.659), 1},
                                SpawnInfo{glm::vec3(78.9564, 13.5452, -113.252), 1},
                                SpawnInfo{glm::vec3(79.0263, 13.5452, -104.222), 1},
                                SpawnInfo{glm::vec3(78.7498, 13.5452, -90.4555), 1},
                                SpawnInfo{glm::vec3(18.6634, 13.5443, -89.0945), 1},
                                SpawnInfo{glm::vec3(18.9138, 13.5443, -100.191), 1},
                                SpawnInfo{glm::vec3(18.6658, 13.5452, -113.078), 1},
                                SpawnInfo{glm::vec3(18.4099, 13.5452, -126.178), 1},
                                SpawnInfo{glm::vec3(29.9228, 0.101, -88.0188), 2},
                                SpawnInfo{glm::vec3(28.7888, 0.101, -123.069), 2},
                                SpawnInfo{glm::vec3(68.6849, 0.101, -122.885), 2},
                                SpawnInfo{glm::vec3(68.9255, 0.101, -90.6996), 2},
                               });
    spawner6Config->insertWave({SpawnInfo{glm::vec3(57.3392, 0.100052, -120.794), 0},
                                SpawnInfo{glm::vec3(51.266, 0.101, -120.457), 0},
                                SpawnInfo{glm::vec3(44.6588, 0.101, -120.532), 0},
                                SpawnInfo{glm::vec3(33.2848, 0.10038, -120.613), 0},
                                SpawnInfo{glm::vec3(33.2588, 0.101, -115.622), 0},
                                SpawnInfo{glm::vec3(41.623, 0.101, -115.515), 0},
                                SpawnInfo{glm::vec3(52.4713, 0.101, -115.662), 0},
                                SpawnInfo{glm::vec3(59.8351, 0.100893, -115.289), 0},
                                SpawnInfo{glm::vec3(60.4392, 0.101, -108.439), 0},
                                SpawnInfo{glm::vec3(54.4908, 0.101, -108.603), 0},
                                SpawnInfo{glm::vec3(42.2557, 0.101, -108.499), 0},
                                SpawnInfo{glm::vec3(33.6413, 0.101, -108.408), 0},
                                SpawnInfo{glm::vec3(32.6087, 0.100481, -101.14), 0},
                                SpawnInfo{glm::vec3(42.0902, 0.101, -100.361), 0},
                                SpawnInfo{glm::vec3(54.5687, 0.101, -99.4731), 0},
                                SpawnInfo{glm::vec3(63.778, 0.101, -99.3862), 0},
                                SpawnInfo{glm::vec3(29.9228, 0.101, -88.0188), 2},
                                SpawnInfo{glm::vec3(28.7888, 0.101, -123.069), 2},
                                SpawnInfo{glm::vec3(68.6849, 0.101, -122.885), 2},
                                SpawnInfo{glm::vec3(68.9255, 0.101, -90.6996), 2},
                               });
    spawner6Config->insertWave({SpawnInfo{glm::vec3(35.7917, 0.101, -93.0488), 2},
                                SpawnInfo{glm::vec3(48.1733, 0.101, -93.1491), 2},
                                SpawnInfo{glm::vec3(60.635, 0.101, -93.247), 2},
                                SpawnInfo{glm::vec3(61.3584, 0.101, -106.974), 2},
                                SpawnInfo{glm::vec3(32.8885, 0.101, -108.573), 2},
                                SpawnInfo{glm::vec3(33.4155, 0.101, -117.516), 2},
                                SpawnInfo{glm::vec3(48.5706, 0.100354, -118.189), 2},
                                SpawnInfo{glm::vec3(66.1708, 0.101, -118.29), 2},
                               });
    spawner6->addComponent<SpawnerComponent>(spawner6Config);
    gameWorld->addGameObject(spawner6);

    addDoor(glm::vec3(-40.1486, 0.11, -0.113809), spawner1->ID(), 0);
    addDoor(glm::vec3(-66.0963, 0.11, -61.8499), spawner2->ID(), M_PI / 2);
    addDoor(glm::vec3(2.95354, 20.9, -87.6664), spawner3->ID(), 0);
    addDoor(glm::vec3(6.33043, 13.6, -9.58123), spawner4->ID(), M_PI / 2);
    addDoor(glm::vec3(47.6875, 0.11, -8.79499), spawner5->ID(), M_PI / 2);
    addDoor(glm::vec3(47.7746, 0.11, -140.262), spawner6->ID(), M_PI / 2);

    addCheckpoint(glm::vec3(-52.1211, 1.101, 0.898749), glm::rotate(glm::mat4(1), float(-M_PI/2), glm::vec3(0, 1, 0)));
    addCheckpoint(glm::vec3(-67.0782, 1.101, -70.5468));
    addCheckpoint(glm::vec3(6.3061, 21.8484, -72.3462));
    addCheckpoint(glm::vec3(6.69662, 14.4577, 5.50677));
    addCheckpoint(glm::vec3(47.7569, 1.101, -40.2404));

    addShop(glm::vec3(-66.5016, 0.101, -74.4995));
    addShop(glm::vec3(6.5294, 20.8716, -95.0435));
    addShop(glm::vec3(5.77519, 13.4577, 16.5316));
    addShop(glm::vec3(47.158, 0.101, -28.6069));

    std::shared_ptr<ModelTransform> trophyTransform = std::make_shared<ModelTransform>();
    trophyTransform->setPos(glm::vec3(46.6353, 0.101, -187.624) + glm::vec3(0, 1, 0));
    std::shared_ptr<GameObject> trophyObj = CreateBaseStaticObject(trophyTransform, hack, "trophyMesh");
    trophyObj->removeComponent<TriangleCollisionComponent>();
    std::shared_ptr<TrophyComponent> trophyComponent = std::make_shared<TrophyComponent>();
    trophyObj->addComponent<TrophyComponent>(trophyComponent);
    trophyObj->getComponent<DrawableComponent>()->setMaterial("bulletTexture");
    m_gameWorld.lock()->addGameObject(trophyObj);

    // These need to be last for all meshes to be included in the collision system!
    gameWorld->getSystem<CollisionSystem>()->collectTriangles();
    gameWorld->getSystem<NavMeshSystem>()->loadNavmesh(navmeshObj->ID());
}

void GameInitializerSystem::addPlayer() {
    std::shared_ptr<GameObject> playerObject = m_gameWorld.lock()->getGameObject(playerID);
    playerObject->getComponent<PlayerComponent>()->setMovementSpeed(7.0f);
    playerObject->getComponent<PlayerComponent>()->setJumpPower(7.0f);
    std::shared_ptr<ModelTransform> playerStart = std::make_shared<ModelTransform>();
    playerStart->setPos(glm::vec3(0, 1, 0));
    playerObject->getComponent<TransformComponent>()->setDefaultTransform(playerStart);
    playerObject->getComponent<EllipsoidCollisionComponent>()->setRadii(glm::vec3(0.25, 0.5, 0.25));
    playerObject->getComponent<EllipsoidCollisionComponent>()->setLocalPos(glm::vec3(0, 0.5, 0));
    playerObject->getComponent<CylinderCollisionComponent>()->setRadius(0.25);
    playerObject->getComponent<DrawableComponent>()->getLocalTransform()->setPos(glm::vec3(0, 0.5, 0));
    playerObject->getComponent<DrawableComponent>()->getLocalTransform()->setScale(glm::vec3(0.5, 1, 0.5));
    playerObject->removeComponent<DrawableComponent>();
    std::shared_ptr<DrawableAnimationComponent> playerAnimationComponent = std::make_shared<DrawableAnimationComponent>(playerAnimated);
    playerAnimationComponent->getLocalTransform()->setScale(0.2f);
    playerAnimationComponent->setAnimation(playerAnimation);
    playerObject->addComponent<DrawableAnimationComponent>(playerAnimationComponent);
    m_gameWorld.lock()->getSystem<DrawAnimationSystem>()->addGameObject(playerObject);

    std::shared_ptr<GameObject> gunObject = std::make_shared<GameObject>();

    std::shared_ptr<DrawableComponent> gunBarrel = std::make_shared<DrawableComponent>();
    gunBarrel->setShape("gunMesh");
    gunBarrel->setMaterial("gunTexture");
    gunBarrel->getLocalTransform()->setScale(glm::vec3(0.07, 0.07, 0.07));
    gunBarrel->getLocalTransform()->setPos(glm::vec3(-0.13, 0.05, 0.3));
    gunBarrel->getLocalTransform()->setRotation(M_PI / 2, glm::vec3(0, 1, 0));
    gunObject->addComponent<DrawableComponent>(gunBarrel);

    std::shared_ptr<GunComponent> gunConfig = std::make_shared<GunComponent>(playerID);
    gunObject->addComponent<GunComponent>(gunConfig);

    std::shared_ptr<TransformComponent> gunTransform = std::make_shared<TransformComponent>();
    gunObject->addComponent<TransformComponent>(gunTransform);

    m_gameWorld.lock()->addGameObject(gunObject);

    std::shared_ptr<ModelTransform> wallCheckTransform = std::make_shared<ModelTransform>();
    wallCheckTransform->setPos(glm::vec3(0, 1, 0));
    std::shared_ptr<GameObject> wallCheckObject = InitializerSystem::CreateBaseKineticObject(wallCheckTransform);
    wallCheckObject->getComponent<RigidBodyComponent>()->setAffectedByGravity(false);
    wallCheckObject->getComponent<EllipsoidCollisionComponent>()->setRadii(glm::vec3(1, 1, 1));
    wallCheckObject->removeComponent<DrawableComponent>();

    m_gameWorld.lock()->addGameObject(wallCheckObject);
    m_gameWorld.lock()->getSystem<SpecialCharacterControllerSystem>()->setWallCheckID(wallCheckObject->ID());
}

void GameInitializerSystem::addBullet(std::shared_ptr<ModelTransform> bulletModel) {
    std::shared_ptr<GameObject> bulletObject = std::make_shared<GameObject>();
    std::shared_ptr<TransformComponent> bulletTransform = std::make_shared<TransformComponent>();
    glm::vec3 bulletForward = glm::vec3(bulletModel->getRotation() * glm::vec4(0, 0, 1, 0));
    bulletTransform->setDefaultTransform(bulletModel);
    bulletTransform->getGlobalTransform()->translate(0.2f * bulletForward);
    bulletObject->addComponent<TransformComponent>(bulletTransform);
    std::shared_ptr<RigidBodyComponent> bulletRigidBody = std::make_shared<RigidBodyComponent>();
    bulletRigidBody->setAffectedByGravity(false);
    bulletObject->addComponent<RigidBodyComponent>(bulletRigidBody);
    std::shared_ptr<CylinderCollisionComponent> bulletCollider = std::make_shared<CylinderCollisionComponent>();
    bulletCollider->setRadius(1);
    bulletCollider->setHeight(0.2);
    bulletObject->addComponent<CylinderCollisionComponent>(bulletCollider);
    std::shared_ptr<EllipsoidCollisionComponent> bulletEllipsoidCollider = std::make_shared<EllipsoidCollisionComponent>();
    bulletEllipsoidCollider->setRadii(glm::vec3(0.2, 0.2, 0.2));
    bulletObject->addComponent<EllipsoidCollisionComponent>(bulletEllipsoidCollider);
    std::shared_ptr<DrawableComponent> bulletDraw = std::make_shared<DrawableComponent>();
    bulletDraw->setShape("cylinder");
    bulletDraw->setMaterial("bulletTexture");
    bulletDraw->getLocalTransform()->setScale(glm::vec3(0.1, 0.15, 0.1));
    bulletDraw->getLocalTransform()->setRotation(M_PI / 2, glm::vec3(1, 0, 0));
    bulletObject->addComponent<DrawableComponent>(bulletDraw);
    std::shared_ptr<BulletComponent> bulletConfig = std::make_shared<BulletComponent>();
    bulletObject->addComponent<BulletComponent>(bulletConfig);
    bulletRigidBody->setVelocity(bulletObject->getComponent<BulletComponent>()->getSpeed() * bulletForward);

    m_gameWorld.lock()->addGameObject(bulletObject);
}

void GameInitializerSystem::addCheckpoint(glm::vec3 pos, glm::mat4 rot) {
    std::shared_ptr<ModelTransform> checkpointTransform = std::make_shared<ModelTransform>();
    checkpointTransform->setPos(pos);
    std::shared_ptr<GameObject> checkpointObj = CreateBaseStaticObject(checkpointTransform, hack, "cube");
    checkpointObj->removeComponent<TriangleCollisionComponent>();
    std::shared_ptr<CheckpointComponent> checkpointComponent = std::make_shared<CheckpointComponent>(pos, rot);
    checkpointObj->addComponent<CheckpointComponent>(checkpointComponent);
    checkpointObj->getComponent<DrawableComponent>()->setMaterial("checkpointTexture");
    m_gameWorld.lock()->addGameObject(checkpointObj);
}

void GameInitializerSystem::addShop(glm::vec3 pos) {
    std::shared_ptr<ModelTransform> shopTransform = std::make_shared<ModelTransform>();
    shopTransform->setPos(pos + glm::vec3(0, 1, 0));
    std::shared_ptr<GameObject> shopObj = CreateBaseStaticObject(shopTransform, hack, "cube");
    shopObj->removeComponent<TriangleCollisionComponent>();
    std::shared_ptr<ShopComponent> shopComponent = std::make_shared<ShopComponent>();
    shopObj->addComponent<ShopComponent>(shopComponent);
    shopObj->getComponent<DrawableComponent>()->setMaterial("shopTexture");
    m_gameWorld.lock()->addGameObject(shopObj);
}

void GameInitializerSystem::addDoor(glm::vec3 doorPos, GameObjectID lockID, float doorRotation) {
    std::shared_ptr<ModelTransform> door1Transform = std::make_shared<ModelTransform>();
    door1Transform->setScale(glm::vec3(1, 1, 1));
    door1Transform->setPos(doorPos);
    std::shared_ptr<GameObject> door1 = CreateBaseKineticObject(door1Transform);
    std::shared_ptr<DoorComponent> door1Config = std::make_shared<DoorComponent>(lockID);
    door1->addComponent<DoorComponent>(door1Config);
    door1->getComponent<DrawableComponent>()->setMaterial("doorTexture");
    door1->getComponent<DrawableComponent>()->setShape("doorMesh");
    door1->getComponent<DrawableComponent>()->getLocalTransform()->setScale(glm::vec3(1.f, 3.5f, 2.f));
    door1->getComponent<DrawableComponent>()->getLocalTransform()->setPos(glm::vec3(0, -1, 0));
    door1->getComponent<DrawableComponent>()->getLocalTransform()->setRotation(doorRotation, glm::vec3(0, 1, 0));
    door1->getComponent<CylinderCollisionComponent>()->makeImmovable();
    door1->getComponent<CylinderCollisionComponent>()->setRadius(2.f);
    door1->getComponent<CylinderCollisionComponent>()->setHeight(3.f);
    door1->getComponent<CylinderCollisionComponent>()->setLocalPos(glm::vec3(0, 1.5f, 0));
    m_gameWorld.lock()->addGameObject(door1);
}

void GameInitializerSystem::addEnemy(glm::vec3 enemyPos, int enemyType) {
//    std::cout << "Added enemy at: " << enemyPos.x << "," << enemyPos.y << "," << enemyPos.z << std::endl;
    std::shared_ptr<ModelTransform> enemyTransform = std::make_shared<ModelTransform>();
    enemyTransform->setPos(enemyPos);
    std::shared_ptr<GameObject> enemyObject = CreateBaseKineticObject(enemyTransform);
    std::shared_ptr<EnemyComponent> enemyConfig = std::make_shared<EnemyComponent>(playerID);
    enemyConfig->enemyType = enemyType;
    enemyObject->addComponent<EnemyComponent>(enemyConfig);
    enemyObject->removeComponent<DrawableComponent>();
    enemyObject->getComponent<CylinderCollisionComponent>()->setHeight(2.0f);
    enemyObject->getComponent<CylinderCollisionComponent>()->setLocalPos(glm::vec3(0, 1, 0));

    std::shared_ptr<GameObject> playerObject = m_gameWorld.lock()->getGameObject(playerID);
    std::shared_ptr<Selector> enemyBTRoot = std::make_shared<Selector>();
    std::shared_ptr<DrawableAnimationComponent> enemyAnimationComponent;

    if (enemyType == 0) {
        std::shared_ptr<AnimatedModel> entity = AnimatedEntityLoader::loadEntity("Resources/Animations/robotArunning.dae", "robotTexture");
        enemyAnimationComponent = std::make_shared<DrawableAnimationComponent>(entity);
        enemyAnimationComponent->getLocalTransform()->setRotation(M_PI, glm::vec3(0,1,0));
        enemyAnimationComponent->getLocalTransform()->setScale(0.5f);
        enemyAnimationComponent->setAnimation(enemyAAnimation);

        std::shared_ptr<Sequence> hitSeq = std::make_shared<Sequence>();
        std::shared_ptr<Sequence> lungeSeq = std::make_shared<Sequence>();
        std::shared_ptr<Sequence> pursueSeq = std::make_shared<Sequence>();
        std::shared_ptr<ActionPursuePlayer> actionPursue = std::make_shared<ActionPursuePlayer>(enemyObject, playerObject, m_gameWorld.lock()->getSystem<NavMeshSystem>(), enemyAnimationComponent->getEntity(), enemyAAnimation);
        std::shared_ptr<ActionLungeAtPlayer> actionLunge = std::make_shared<ActionLungeAtPlayer>(enemyObject, playerObject, entity, enemyADashAnimation);
        std::shared_ptr<ConditionPlayerIsClose> conditionClosest = std::make_shared<ConditionPlayerIsClose>(enemyObject, playerObject, 4.f);
        std::shared_ptr<ConditionPlayerIsClose> conditionClose = std::make_shared<ConditionPlayerIsClose>(enemyObject, playerObject, 15.f);
        std::shared_ptr<ActionHit> actionHit = std::make_shared<ActionHit>(enemyObject, playerObject, m_gameWorld.lock()->getSystem<HealthSystem>(), entity, enemyAHitAnimation);
        hitSeq->setChildren({conditionClosest, actionHit});
        lungeSeq->setChildren({conditionClose, actionLunge});
        pursueSeq->setChildren({actionPursue});
        enemyBTRoot->setChildren({hitSeq, lungeSeq, pursueSeq});

    }
    else if (enemyType == 1) {
        std::shared_ptr<AnimatedModel> entity = AnimatedEntityLoader::loadEntity("Resources/Animations/robotBrunning.dae", "robotTexture");
        enemyAnimationComponent = std::make_shared<DrawableAnimationComponent>(entity);
        enemyAnimationComponent->getLocalTransform()->setRotation(M_PI, glm::vec3(0,1,0));
        enemyConfig->setMaxHP(4);

        enemyAnimationComponent->getLocalTransform()->setScale(0.5f);
        enemyAnimationComponent->setAnimation(enemyBAnimation);

        std::shared_ptr<Sequence> runSeq = std::make_shared<Sequence>();
        std::shared_ptr<Sequence> pursueSeq = std::make_shared<Sequence>();
        std::shared_ptr<Sequence> shootSeq = std::make_shared<Sequence>();
        std::shared_ptr<ActionRunAway> actionRun = std::make_shared<ActionRunAway>(enemyObject, playerObject, m_gameWorld.lock()->getSystem<NavMeshSystem>(), enemyAnimationComponent->getEntity(), enemyBAnimation);
        std::shared_ptr<ConditionPlayerIsClose> conditionClose = std::make_shared<ConditionPlayerIsClose>(enemyObject, playerObject, 5.f);
        runSeq->setChildren({conditionClose, actionRun});
        std::shared_ptr<ActionPursuePlayer> actionPursue = std::make_shared<ActionPursuePlayer>(enemyObject, playerObject, m_gameWorld.lock()->getSystem<NavMeshSystem>(), enemyAnimationComponent->getEntity(), enemyBAnimation);
        std::shared_ptr<ConditionPlayerIsFar> conditionFar = std::make_shared<ConditionPlayerIsFar>(enemyObject, playerObject, 30.f, m_gameWorld.lock()->getSystem<CollisionSystem>());
        pursueSeq->setChildren({conditionFar, actionPursue});
        std::shared_ptr<ActionShootPlayer> actionShoot = std::make_shared<ActionShootPlayer>(enemyObject, playerObject, m_gameWorld, enemyAnimationComponent->getEntity(), enemyBFiringAnimation);
        shootSeq->setChildren({actionShoot});
        enemyBTRoot->setChildren({runSeq, pursueSeq, shootSeq});
    }

    else if (enemyType == 2) {
        std::shared_ptr<AnimatedModel> entity = AnimatedEntityLoader::loadEntity("Resources/Animations/robotCrunning.dae", "robotTexture");
        enemyAnimationComponent = std::make_shared<DrawableAnimationComponent>(entity);
        enemyAnimationComponent->getLocalTransform()->setRotation(M_PI, glm::vec3(0,1,0));
        enemyAnimationComponent->getLocalTransform()->setScale(0.7f);
        enemyAnimationComponent->setAnimation(enemyCAnimation);
        enemyConfig->setSpeed(9.f);
        enemyConfig->setMaxHP(20);
        enemyObject->getComponent<CylinderCollisionComponent>()->setHeight(3.0f);
        enemyObject->getComponent<CylinderCollisionComponent>()->setRadius(1.f);
        enemyObject->getComponent<CylinderCollisionComponent>()->setLocalPos(glm::vec3(0, 1.5f, 0));

        std::shared_ptr<Sequence> smackSeq = std::make_shared<Sequence>();
        std::shared_ptr<Sequence> pursueSeq = std::make_shared<Sequence>();
        std::shared_ptr<Sequence> leapSeq = std::make_shared<Sequence>();
        std::shared_ptr<ActionPursuePlayer> actionPursue = std::make_shared<ActionPursuePlayer>(enemyObject, playerObject, m_gameWorld.lock()->getSystem<NavMeshSystem>(), entity, enemyCAnimation);
        std::shared_ptr<ConditionPlayerIsClose> conditionClose = std::make_shared<ConditionPlayerIsClose>(enemyObject, playerObject, 5.f);
        std::shared_ptr<ActionSmack> actionSmack = std::make_shared<ActionSmack>(enemyObject, playerObject, m_gameWorld.lock()->getSystem<HealthSystem>(), entity, enemyCSmackAnimation);
        std::shared_ptr<ConditionPlayerIsFar> conditionFar = std::make_shared<ConditionPlayerIsFar>(enemyObject, playerObject, 10.f, m_gameWorld.lock()->getSystem<CollisionSystem>(), false);
        std::shared_ptr<ActionLeap> actionLeap = std::make_shared<ActionLeap>(enemyObject, playerObject, m_gameWorld.lock()->getSystem<HealthSystem>(), entity, enemyCLeapAnimation, m_gameWorld.lock()->getSystem<CollisionSystem>());

        smackSeq->setChildren({conditionClose, actionSmack});
        pursueSeq->setChildren({actionPursue});
        leapSeq->setChildren({conditionFar, actionLeap});
        enemyBTRoot->setChildren({leapSeq, smackSeq, pursueSeq});
    }

    enemyObject->addComponent<DrawableAnimationComponent>(enemyAnimationComponent);

    std::shared_ptr<AIComponent> enemyAI = std::make_shared<AIComponent>();
    enemyAI->setBehaviorTree(enemyBTRoot);
    enemyObject->addComponent<AIComponent>(enemyAI);

    m_gameWorld.lock()->addGameObject(enemyObject);
}

}
