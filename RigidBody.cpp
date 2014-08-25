#include "RigidBody.h"

#include "SceneNode.h"
#include "Entity.h"
#include "Mesh.h"
#include "PhysicsManager.h"

RigidBody::RigidBody(std::string name, SceneNode* node, Entity* ent)
{
	mName = name;

	//Bullet collision shape, default shape -> Box
	glm::vec3 AABBsize = ent->getMesh()->getAABBsize();
	AABBsize /= 2.0f; //Bullet halfsize
	mBulletCollisionShape = new btBoxShape(btVector3(AABBsize.x, AABBsize.y, AABBsize.z));

	//Bullet motion state
	glm::vec3 position = node->getDerivedPosition();
	glm::quat orientation = node->getOrientation();
	glm::vec3 scale = node->getDerivedScale();

	btTransform trans;

	trans.setIdentity();
	trans.setOrigin(btVector3(position.x, position.y, position.z));
	trans.setRotation(btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w));
	
	mBulletMotionState = new btDefaultMotionState(trans);
	mBulletCollisionShape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));

	//Create knitec body
	btRigidBody::btRigidBodyConstructionInfo btConstructionInfo(0.0, mBulletMotionState, mBulletCollisionShape, btVector3(0.0, 0.0, 0.0));
	mBulletRigidBody = new btRigidBody(btConstructionInfo);
	mBulletRigidBody->setCollisionFlags(mBulletRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT); 
	mBulletRigidBody->setActivationState(DISABLE_DEACTIVATION); //Disable the desactivation

	//Set user pointer
	mBulletRigidBody->setUserPointer(ent);

	//Add the rigid body to the world
	mDynamicWorld = PhysicsManager::getSingletonPtr()->getDynamicWorld();
	mDynamicWorld->addRigidBody(mBulletRigidBody);
}


RigidBody::~RigidBody()
{
	mDynamicWorld->removeRigidBody(mBulletRigidBody);
	delete mBulletRigidBody;
	delete mBulletMotionState;
	delete mBulletCollisionShape;
}


void RigidBody::setTransforms(SceneNode* node)
{
	glm::vec3 position = node->getDerivedPosition();
	glm::quat orientation = node->getOrientation();
	glm::vec3 scale = node->getDerivedScale();

	btTransform trans;

	trans.setIdentity();
	trans.setOrigin(btVector3(position.x, position.y, position.z));
	trans.setRotation(btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w));
	mBulletMotionState->setWorldTransform(trans);

	mBulletCollisionShape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
}

void RigidBody::setMass(real mass)
{
	if (mBulletRigidBody->getInvMass() == 0.0)
	{
		btVector3 inertia = btVector3(0.0, 0.0, 0.0);
		if (mass != 0.0)
		{
			mBulletCollisionShape->calculateLocalInertia(mass, inertia);
		}

		mDynamicWorld->removeRigidBody(mBulletRigidBody);
		mBulletRigidBody->setMassProps(mass, inertia);
		mBulletRigidBody->setCollisionFlags(0); //Remove kinetic object property because the mass is positive
		Entity* ent = static_cast<Entity*>(mBulletRigidBody->getUserPointer());
		mDynamicWorld->addRigidBody(mBulletRigidBody);
	}
}
real RigidBody::getMass()
{
	return mBulletRigidBody->getInvMass();
}

Entity* RigidBody::getUserPointer()
{
	return static_cast<Entity*>(mBulletRigidBody->getUserPointer());
}

void RigidBody::setLinearVelocity(glm::vec3& vel)
{
	mBulletRigidBody->setLinearVelocity(btVector3(vel.x, vel.y, vel.z));
}