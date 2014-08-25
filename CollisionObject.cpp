#include "CollisionObject.h"

#include "PhysicsManager.h"
#include "SceneNode.h"
#include "Entity.h"
#include "Mesh.h"

CollisionObject::CollisionObject(std::string name, SceneNode* node, Entity* ent)
{
	mName = name;

	glm::vec3 AABBsize = ent->getMesh()->getAABBsize();
	AABBsize /= 2.0f; //Bullet halfsize
	mBulletCollisionShape = new btBoxShape(btVector3(AABBsize.x, AABBsize.y, AABBsize.z));
	mBulletCollisionObject = new btCollisionObject();
	mBulletCollisionObject->setCollisionShape(mBulletCollisionShape);

	glm::vec3 position = node->getDerivedPosition();
	glm::quat orientation = node->getOrientation();
	glm::vec3 scale = node->getDerivedScale();

	btTransform trans;

	trans.setIdentity();
	trans.setOrigin(btVector3(position.x, position.y, position.z));
	trans.setRotation(btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w));
	mBulletCollisionObject->setWorldTransform(trans);

	mBulletCollisionShape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));

	mBulletCollisionObject->setUserPointer(ent);

	mDynamicWorld = PhysicsManager::getSingletonPtr()->getDynamicWorld();
	mDynamicWorld->addCollisionObject(mBulletCollisionObject);
}


CollisionObject::~CollisionObject()
{
	mDynamicWorld->removeCollisionObject(mBulletCollisionObject);
	delete mBulletCollisionObject;
	delete mBulletCollisionShape;
}

void CollisionObject::setTransforms(SceneNode* node)
{
	glm::vec3 position = node->getDerivedPosition();
	glm::quat orientation = node->getOrientation();
	glm::vec3 scale = node->getDerivedScale();

	btTransform trans;

	trans.setIdentity();
	trans.setOrigin(btVector3(position.x, position.y, position.z));
	trans.setRotation(btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w));
	mBulletCollisionObject->setWorldTransform(trans);

	mBulletCollisionShape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
}