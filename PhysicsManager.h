#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "Definitions.h"
#include "Singleton.h"

#include <string>
#include <map>

class PhysicsManager: public Singleton<PhysicsManager>
{
public:
	PhysicsManager();
	~PhysicsManager();

	void initPhysicsWorld();

	RigidBody* createRigidBody(std::string name);

private:
	//Bullet engine objects
	btBroadphaseInterface* mBulletBroadphase;
	btDefaultCollisionConfiguration* mBulletCollisionConfig;
	btCollisionDispatcher* mBulletDispatcher;
	btSequentialImpulseConstraintSolver* mBulletSolver;
	btDiscreteDynamicsWorld* mBulletDynamicWorld;

	//Rigid bodies
	typedef std::map<std::string, RigidBody*> tRigidBodiesMap;
	tRigidBodiesMap mRigidBodiesMap;
};

#endif