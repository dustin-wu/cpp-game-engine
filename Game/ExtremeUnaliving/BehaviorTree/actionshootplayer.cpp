#include "actionshootplayer.h"
#include "Engine/Components/CollisionComponents/cylindercollisioncomponent.h"
#include "Engine/Components/CollisionComponents/ellipsoidcollisioncomponent.h"
#include "Engine/Components/drawablecomponent.h"
#include "Engine/Components/rigidbodycomponent.h"
#include "Engine/Components/transformcomponent.h"
#include "Game/ExtremeUnaliving/Components/bulletcomponent.h"

namespace ExtremeUnaliving {

ActionShootPlayer::ActionShootPlayer(std::shared_ptr<GameObject> entity, std::shared_ptr<GameObject> target, std::weak_ptr<GameWorld> gameWorld, std::shared_ptr<AnimatedModel> animModel, std::shared_ptr<Animation> animation)
{
    m_entity = entity;
    m_target = target;
    m_gameWorld = gameWorld;
    coolDown = 2.5f;
    m_animModel = animModel;
    m_animation = animation;
}

Status ActionShootPlayer::act(float deltaTime) {
    m_animModel->doAnimation(m_animation, 2.5f - coolDown);

    std::shared_ptr<ModelTransform> entityModel = m_entity->getComponent<TransformComponent>()->getGlobalTransform();
    std::shared_ptr<ModelTransform> targetModel = m_target->getComponent<TransformComponent>()->getGlobalTransform();

    glm::vec3 entityPos = entityModel->getPos();
    glm::vec3 targetPos = targetModel->getPos() - glm::vec3(0, 0.2, 0);

    glm::vec3 dirBullet = glm::normalize(targetPos - entityPos);
    glm::vec3 target2D = glm::vec3(targetModel->getPos().x, 0, targetModel->getPos().z);
    glm::vec3 entity2D = glm::vec3(entityModel->getPos().x, 0, entityModel->getPos().z);
    glm::vec3 dir = glm::normalize(target2D - entity2D);
    glm::vec3 up = glm::vec3(0, 1, 0);
    glm::vec3 right = glm::normalize(glm::cross(dir, up));

    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0][0] = -right.x;
    rotation[0][1] = -right.y;
    rotation[0][2] = -right.z;
    rotation[1][0] = up.x;
    rotation[1][1] = up.y;
    rotation[1][2] = up.z;
    rotation[2][0] = dir.x;
    rotation[2][1] = dir.y;
    rotation[2][2] = dir.z;

    glm::quat startRotation = entityModel->getRotation();
    glm::quat endRotation = rotation;
    float rotationSpeed = 10.0f;
    glm::quat interpolatedRotation = glm::slerp(startRotation, endRotation, deltaTime * rotationSpeed);
    entityModel->setRotation(glm::mat4(interpolatedRotation));

    coolDown -= deltaTime;
    if (coolDown <= 0) {
        coolDown = 2.5f;;
    } else {
        return Status::RUNNING;
    }

    std::shared_ptr<ModelTransform> bulletTransform = std::make_shared<ModelTransform>();
    bulletTransform->setPos(entityPos + glm::vec3(0, 1.5, 0));
    bulletTransform->setRotation(glm::mat4(interpolatedRotation));

//    std::cout << "shooting player " << deltaTime << std::endl;

    addBullet(bulletTransform, dirBullet);
    return Status::SUCCESS;
}

void ActionShootPlayer::addBullet(std::shared_ptr<ModelTransform> bulletModel, glm::vec3 dir) {
    std::shared_ptr<GameObject> bulletObject = std::make_shared<GameObject>();
    std::shared_ptr<TransformComponent> bulletTransform = std::make_shared<TransformComponent>();
//    bulletTransform->setDefaultTransform(bulletModel);
    bulletTransform->getGlobalTransform()->setPos(m_entity->getComponent<TransformComponent>()->getGlobalTransform()->getPos() + glm::vec3(0,1.5, 0) + 2.f * dir);
    bulletObject->addComponent<TransformComponent>(bulletTransform);
    std::shared_ptr<DrawableComponent> bulletDraw = std::make_shared<DrawableComponent>();
    bulletDraw->setShape("sphere");
    bulletDraw->setMaterial("healthbarTexture");
    bulletDraw->getLocalTransform()->setScale(glm::vec3(0.2, 0.2, 0.2));
    bulletDraw->getLocalTransform()->setRotation(M_PI / 2, glm::vec3(1, 0, 0));
    bulletObject->addComponent<DrawableComponent>(bulletDraw);
    std::shared_ptr<BulletComponent> bulletConfig = std::make_shared<BulletComponent>();
    bulletObject->addComponent<BulletComponent>(bulletConfig);
    std::shared_ptr<RigidBodyComponent> bulletRigidBody = std::make_shared<RigidBodyComponent>();
    bulletRigidBody->setAffectedByGravity(false);
    bulletObject->addComponent<RigidBodyComponent>(bulletRigidBody);
    bulletRigidBody->setVelocity(bulletObject->getComponent<BulletComponent>()->getSpeed() * dir);

    std::shared_ptr<CylinderCollisionComponent> bulletCollider = std::make_shared<CylinderCollisionComponent>(false);
    bulletCollider->setRadius(0.5);
    bulletCollider->setHeight(0.5);
    bulletCollider->setLocalPos(glm::vec3(0, -1, 0));

    bulletObject->addComponent<CylinderCollisionComponent>(bulletCollider);

    std::shared_ptr<EllipsoidCollisionComponent> bulletEllipsoidCollider = std::make_shared<EllipsoidCollisionComponent>();
    bulletEllipsoidCollider->setRadii(glm::vec3(0.1, 0.1, 0.1));
    bulletEllipsoidCollider->physics = false;
    bulletObject->addComponent<EllipsoidCollisionComponent>(bulletEllipsoidCollider);



    m_gameWorld.lock()->addGameObject(bulletObject);
}


}
