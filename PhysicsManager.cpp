#include "PhysicsManager.h"

#include "RigidBody.h"
#include "CollisionObject.h"
#include "SceneNode.h"
#include "Entity.h"

PhysicsManager::PhysicsManager()
{
}


PhysicsManager::~PhysicsManager()
{
	tRigidBodiesMap::iterator rigidBodyIt;
	for (rigidBodyIt = mRigidBodiesMap.begin(); rigidBodyIt != mRigidBodiesMap.end(); ++rigidBodyIt)
	{
		delete rigidBodyIt->second;
	}
	mRigidBodiesMap.clear();

	tCollisionObjectsMap::iterator collisionObjectIt;
	for (collisionObjectIt = mCollisionMap.begin(); collisionObjectIt != mCollisionMap.end(); ++collisionObjectIt)
	{
		delete collisionObjectIt->second;
	}
	mRigidBodiesMap.clear();

	delete mBulletDebugDrawer;
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

	//Drawer
	mBulletDebugDrawer = new GLDebugDrawer();
	mBulletDebugDrawer->setDebugMode(mBulletDebugDrawer->DBG_DrawWireframe);
	mBulletDynamicWorld->setDebugDrawer(mBulletDebugDrawer);
}


RigidBody* PhysicsManager::createRigidBody(std::string name, SceneNode* node, Entity* ent)
{
	tRigidBodiesMap::iterator it = mRigidBodiesMap.find(name);
	if (it != mRigidBodiesMap.end())
	{
		return it->second;
	}

	RigidBody* newRigidBody = new RigidBody(name, node, ent);
	mRigidBodiesMap.insert(std::pair<std::string, RigidBody*>(name, newRigidBody));

	return newRigidBody;
}

void PhysicsManager::deleteRigidBody(std::string name)
{
	tRigidBodiesMap::iterator it = mRigidBodiesMap.find(name);
	if (it != mRigidBodiesMap.end()) //If we find the rigidbody delete it
	{
		delete it->second;
		mRigidBodiesMap.erase(it);
	}
}

CollisionObject* PhysicsManager::createCollisionObject(std::string name, SceneNode* node, Entity* ent)
{
	tCollisionObjectsMap::iterator it = mCollisionMap.find(name);
	if (it != mCollisionMap.end())
	{
		return it->second;
	}

	CollisionObject* newCollisionObject = new CollisionObject(name, node, ent);
	mCollisionMap.insert(std::pair<std::string, CollisionObject*>(name, newCollisionObject));

	return newCollisionObject;
}

void PhysicsManager::processPhysicsWorld(real deltaTime)
{
	//Simulate the world
	mBulletDynamicWorld->stepSimulation(deltaTime, 10.0);

	//Update scenenode positions
	tRigidBodiesMap::iterator it;
	for (it = mRigidBodiesMap.begin(); it != mRigidBodiesMap.end(); ++it)
	{
		if (it->second->getType() == RigidBody::DYNAMIC)
		{
			btTransform worldTrans;
			it->second->getTransforms(worldTrans);
			btVector3 pos = worldTrans.getOrigin();
			btQuaternion rot = worldTrans.getRotation();

			it->second->getUserPointer()->getAttachedSceneNode()->setPosition(glm::vec3(pos.x(), pos.y(), pos.z()));
			it->second->getUserPointer()->getAttachedSceneNode()->setOrientation(glm::quat(rot.w(), rot.x(), rot.y(), rot.z()));
		}
		else if (it->second->getType() == RigidBody::KINETIC)
		{
			it->second->setTransforms();
		}
	}
}

void PhysicsManager::draw()
{
	mBulletDynamicWorld->debugDrawWorld();
}
