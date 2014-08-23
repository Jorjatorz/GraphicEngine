#include "RayCast.h"

#include "PhysicsManager.h"

RayCast::RayCast()
{
	mCurrentDynamicWolrd = PhysicsManager::getSingletonPtr()->getDynamicWorld();

	hasHits = false;
	mHitObject = nullptr;
}


RayCast::~RayCast()
{
}
#include <iostream>
#include "MovableObject.h"
#include "Entity.h"
void RayCast::executeRaySelectionCast(glm::vec3 start, glm::vec3 end)
{
	btCollisionWorld::ClosestRayResultCallback rayCallBack(btVector3(start.x, start.y, start.z), btVector3(end.x, end.y, end.z));

	mCurrentDynamicWolrd->rayTest(btVector3(start.x, start.y, start.z), btVector3(end.x, end.y, end.z), rayCallBack);

	if (rayCallBack.hasHit())
	{
		hasHits = true;

		mHitObject = static_cast<Entity*>(rayCallBack.m_collisionObject->getUserPointer());

		mHitObject->showAABB(!mHitObject->isAABBshowed());
	}
	else
	{
		hasHits = false;
		mHitObject = nullptr;
	}
}
