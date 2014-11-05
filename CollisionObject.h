#ifndef COLLISIONOBJECT_H
#define COLLISIONOBJECT_H

#include "Definitions.h"

class CollisionObject
{
public:
	//Creates a new collision object with a pointer to the Entity ent attached to the SceneNode node
	CollisionObject(std::string name, SceneNode* node, Entity* ent);
	~CollisionObject();

	//Updates the transformation matrix of the collision object from the Scenenode node
	void setTransforms(SceneNode* node);

private:
	//Bullet staff - All created in the constructor (DynamicWorld is already created, just save a reference to it)
	btDiscreteDynamicsWorld* mDynamicWorld;
	btCollisionObject* mBulletCollisionObject;
	btCollisionShape* mBulletCollisionShape;

	//Collision object name
	std::string mName;
};

#endif