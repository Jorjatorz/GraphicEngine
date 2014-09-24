#include "RayCast.h"

#include "PhysicsManager.h"
#include "MovableObject.h"

RayCast::RayCast()
{
	mCurrentDynamicWolrd = PhysicsManager::getSingletonPtr()->getDynamicWorld();

	bHits = false;
	mHitObject = NULL;
}


RayCast::~RayCast()
{
}

void RayCast::executeRaySelectionCast(glm::vec3 start, glm::vec3 end)
{
	btCollisionWorld::ClosestRayResultCallback rayCallBack(btVector3(start.x, start.y, start.z), btVector3(end.x, end.y, end.z));

	mCurrentDynamicWolrd->rayTest(btVector3(start.x, start.y, start.z), btVector3(end.x, end.y, end.z), rayCallBack);

	if (rayCallBack.hasHit())
	{
		bHits = true;

		mHitObject = static_cast<MovableObject*>(rayCallBack.m_collisionObject->getUserPointer());

		//std::cout << mHitObject->getName() << std::endl;
	}
	else
	{
		bHits = false;
		mHitObject = NULL;
	}
}
