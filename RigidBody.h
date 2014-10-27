#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Definitions.h"

#include <string>

class RigidBody
{
public:
	RigidBody(std::string name, SceneNode* node, Entity* ent);
	~RigidBody();
	
	//Types
	typedef enum
	{
		DYNAMIC,
		KINETIC,
		STATIC
	} tRigidBodyType;

	typedef enum//Can add more
	{
		BOX_SHAPE_,
		SPHERE_SHAPE_
	} tRigidBodyShape;

	//Properties
	tRigidBodyType getType()
	{
		return mRigidBodyType;
	}
	void setMass(real mass, bool setStatic = false); //This function sets the mass of the rigidbody, this means that it transforms the body into kinetic, static or dynamic object
	void setMass(real mass, tRigidBodyType type); //Same as previous, but passing the type
	real getMass();
	void setLinearVelocity(glm::vec3& vel);
	void addRigidBodyToWorld();
	void removeRigidBodyFromWorld();
	void makeRigidBodyWithNoCollisions();
	//Shape change WARNING: Change it before applying mass (cant acoid this inse the function with code and setMass)
	void setShape_Box(glm::vec3& boxDimensions);
	void setShape_Sphere(real radius);

	//Transforms
	void setTransforms(SceneNode* node); //Just for kinetic objects, called when attached to a scenenode and when the entity is processed
	void getTransforms(btTransform& trans)
	{
		mBulletMotionState->getWorldTransform(trans);
	}

	
	Entity* getUserPointer();

private:
	std::string mName;
	Entity* mEntity; //Entity attached to
	tRigidBodyType mRigidBodyType;
	tRigidBodyShape mRigidBodyShape;

	//Type changers
	void setType_Dynamic();
	void setType_Kinetic();
	void setType_Static();

	//Bullet staff
	btDiscreteDynamicsWorld* mDynamicWorld; //Ptr to the dynamic world
	btCollisionShape* mBulletCollisionShape;
	btDefaultMotionState* mBulletMotionState;
	btRigidBody* mBulletRigidBody;

	//Collision and physics
	bool mInPhysicsWorld;
};

#endif