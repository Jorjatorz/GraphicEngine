#ifndef COLLISIONOBJECT_H
#define COLLISIONOBJECT_H

#include "Definitions.h"

class CollisionObject
{
public:
	CollisionObject(std::string name, SceneNode* node, Entity* ent);
	~CollisionObject();

	void setTransforms(SceneNode* node);

private:
	btDiscreteDynamicsWorld* mDynamicWorld;
	btCollisionObject* mBulletCollisionObject;
	btCollisionShape* mBulletCollisionShape;

	std::string mName;
};

#endif