#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Definitions.h"

#include <string>

class RigidBody
{
public:
	RigidBody(std::string name, SceneNode* node, Entity* ent);
	~RigidBody();

	//Properties
	void setMass(real mass);
	real getMass();
	void setLinearVelocity(glm::vec3& vel);

	//Transforms
	void setTransforms(SceneNode* node); //Just for kinetic objects
	void getTransforms(btTransform& trans)
	{
		mBulletMotionState->getWorldTransform(trans);
	}

	Entity* getUserPointer();

private:
	std::string mName;

	//Bullet staff
	btDiscreteDynamicsWorld* mDynamicWorld; //Ptr to the dynamic world
	btCollisionShape* mBulletCollisionShape;
	btDefaultMotionState* mBulletMotionState;
	btRigidBody* mBulletRigidBody;
};

#endif