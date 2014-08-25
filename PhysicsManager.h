#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "Definitions.h"
#include "Singleton.h"

#include <string>
#include <map>

#include "GLDebugDrawer.h"
class PhysicsManager: public Singleton<PhysicsManager>
{
public:
	PhysicsManager();
	~PhysicsManager();

	void initPhysicsWorld();

	void draw();

	void processPhysicsWorld(real deltaTime);

	RigidBody* createRigidBody(std::string name);

	btDiscreteDynamicsWorld* getDynamicWorld()
	{
		return mBulletDynamicWorld;
	}

private:
	//Bullet engine objects
	btBroadphaseInterface* mBulletBroadphase;
	btDefaultCollisionConfiguration* mBulletCollisionConfig;
	btCollisionDispatcher* mBulletDispatcher;
	btSequentialImpulseConstraintSolver* mBulletSolver;
	btDiscreteDynamicsWorld* mBulletDynamicWorld;
	btIDebugDraw* mBulletDebugDrawer;

	//Rigid bodies
	typedef std::map<std::string, RigidBody*> tRigidBodiesMap;
	tRigidBodiesMap mRigidBodiesMap;
};

#endif