#include "PhysicsManager.h"

#include "RigidBody.h"

PhysicsManager::PhysicsManager()
{
}


PhysicsManager::~PhysicsManager()
{
	delete mBulletDynamicWorld;
	delete mBulletSolver;
	delete mBulletDispatcher;
	delete mBulletCollisionConfig;
	delete mBulletBroadphase;
}

void PhysicsManager::initPhysicsWorld()
{
	mBulletBroadphase = new btDbvtBroadphase(); //The broadphase is an excellent place for eliminating object pairs that should not collide.
	//The collision configuration allows you to fine tune the algorithms used for the full (not broadphase) collision detection.
	mBulletCollisionConfig = new btDefaultCollisionConfiguration();
	mBulletDispatcher = new btCollisionDispatcher(mBulletCollisionConfig);

	btGImpactCollisionAlgorithm::registerAlgorithm(mBulletDispatcher);	//f you introduce different types of collision object later (eg meshes using btGImpactMeshShape) then you may need to register a collision algorithm to get collisions recognised
	
	mBulletSolver = new btSequentialImpulseConstraintSolver;//This is what causes the objects to interact properly, taking into account gravity, game logic supplied forces, collisions, and hinge constraints.

	mBulletDynamicWorld = new btDiscreteDynamicsWorld(mBulletDispatcher, mBulletBroadphase, mBulletSolver, mBulletCollisionConfig);

	//Set the gravity
	mBulletDynamicWorld->setGravity(btVector3(0.0, -9.8, 0.0));
}


RigidBody* PhysicsManager::createRigidBody(std::string name)
{
	tRigidBodiesMap::iterator it = mRigidBodiesMap.find(name);

	if (it != mRigidBodiesMap.end())
	{
		return it->second;
	}

	RigidBody* newRigidBody = new RigidBody(name, mBulletDynamicWorld);

	mRigidBodiesMap.insert(std::pair<std::string, RigidBody*>(name, newRigidBody));

	return newRigidBody;
}