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
	mRigidBodyShape = BOX_SHAPE_;

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
	//Trying---- Continuos collision detection
	mBulletRigidBody->setCcdMotionThreshold(1);
	mBulletRigidBody->setCcdSweptSphereRadius(0.8);
	//----
	setType_Static(); //Default state

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
void RigidBody::setType_Dynamic()
{
	//Set the activation type
	mBulletRigidBody->forceActivationState(ACTIVE_TAG);
	mBulletRigidBody->setCollisionFlags(0);

	mRigidBodyType = DYNAMIC;
}
void RigidBody::setType_Kinetic()
{
	//Set the activation type
	mBulletRigidBody->setActivationState(DISABLE_DEACTIVATION);
	mBulletRigidBody->setCollisionFlags(mBulletRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);

	mRigidBodyType = KINETIC;
}
void RigidBody::setType_Static()
{
	//Set the activation type
	mBulletRigidBody->forceActivationState(ACTIVE_TAG);
	mBulletRigidBody->setCollisionFlags(0);

	mRigidBodyType = STATIC;
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
void RigidBody::setMass(real mass, bool setStatic)
{
	if (mass != 0.0)
	{
		mDynamicWorld->removeRigidBody(mBulletRigidBody);
		
		btVector3 inertia;
		mBulletCollisionShape->calculateLocalInertia(mass, inertia);
		mBulletRigidBody->setMassProps(mass, inertia);
		setType_Dynamic();

		mDynamicWorld->addRigidBody(mBulletRigidBody);
	}
	else
	{
		if (setStatic)
		{
			mDynamicWorld->removeRigidBody(mBulletRigidBody);
			mBulletRigidBody->setMassProps(0.0, btVector3(0.0, 0.0, 0.0));
			setType_Static();
			mDynamicWorld->addRigidBody(mBulletRigidBody);
		}
		else
		{
			mDynamicWorld->removeRigidBody(mBulletRigidBody);
			mBulletRigidBody->setMassProps(0.0, btVector3(0.0, 0.0, 0.0));
			setType_Kinetic();
			mDynamicWorld->addRigidBody(mBulletRigidBody);
		}
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
void RigidBody::setShape_Box(glm::vec3& boxDimensions)
{
	if (mRigidBodyShape == SPHERE_SHAPE_)
	{
		delete mBulletCollisionShape; //delete the old shape

		boxDimensions /= 2.0f; //Bullet half-size
		mBulletCollisionShape = new btBoxShape(btVector3(boxDimensions.x, boxDimensions.y, boxDimensions.z));
		mRigidBodyShape = BOX_SHAPE_;
	}
	else //If the shape is the box return
	{
		return;
	}

	mBulletRigidBody->setCollisionShape(mBulletCollisionShape);
}
void RigidBody::setShape_Sphere(real radius)
{
	if (mRigidBodyShape == BOX_SHAPE_)
	{
		delete mBulletCollisionShape; //delete the old shape

		mBulletCollisionShape = new btSphereShape(radius);
		mRigidBodyShape = SPHERE_SHAPE_;
	}
	else //If the shape is the spehre return
	{
		return;
	}

	mBulletRigidBody->setCollisionShape(mBulletCollisionShape);
}