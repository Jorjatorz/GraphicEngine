#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Definitions.h"

#include <string>

class RigidBody
{
public:
	RigidBody(std::string name, btDiscreteDynamicsWorld* theWorld);
	~RigidBody();

	void setUpRigidBody(real mass, SceneNode* node, Entity* ent);

	void setCollisionShape_Box(glm::vec3& boxDimensions);
	void setCollisionShape_Sphere(real radius);
	void setCollisionShape_Plane(glm::vec3& normal, real origin);
	void setCollisionShape_ConvexHull(Mesh* model);

	Entity* getUserPointer()
	{
		return static_cast<Entity*>(mBulletRigidBody->getUserPointer());
	}
	void getWorldTransform(btTransform& trans)
	{
		return mBulletMotionState->getWorldTransform(trans);
	}
	real getMass()
	{
		return mMass;
	}

	void setLinearVelocity(glm::vec3& a)
	{
		mBulletRigidBody->setLinearVelocity(btVector3(a.x, a.y, a.z));
	}

private:
	std::string mName;

	real mMass;

	bool rigidBodySetUp_;
	bool collisionShapeSetUp_;

	//for box shape
	glm::vec3 mBoxDimensions;
	//for sphere shape
	real mRadius;
	//for plane shape
	glm::vec3 mNormal;
	real mOrigin;

	//Bullet staff
	btDiscreteDynamicsWorld* mCurrentDynamicWorld; //Ptr to the dynamic world
	btCollisionShape* mBulletCollisionShape;
	btDefaultMotionState* mBulletMotionState;
	btRigidBody* mBulletRigidBody;
};

#endif