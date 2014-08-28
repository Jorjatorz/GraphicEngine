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
	void processPhysicsWorld(real deltaTime);

	//DEbug draw
	void draw();


	//Rigidbodies
	RigidBody* createRigidBody(std::string name, SceneNode* node, Entity* ent);
	void deleteRigidBody(std::string name);

	//Collisionobjects
	CollisionObject* createCollisionObject(std::string name, SceneNode* node, Entity* ent);

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
	//Collision object
	typedef std::map<std::string, CollisionObject*> tCollisionObjectsMap;
	tCollisionObjectsMap mCollisionMap;
};

#endif