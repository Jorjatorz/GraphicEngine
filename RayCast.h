#ifndef RAYCAST_H
#define RAYCAST_H

#include "Definitions.h"

class RayCast
{
public:
	RayCast();
	~RayCast();

	//For raycast from camera using mouse pass as parameter -> start = cameraPos, end = mouseWorldPos * X big number
	void executeRaySelectionCast(glm::vec3 start, glm::vec3 end); //End can be used as direction multiplaying the direction by a high number

	//Return the closes hit object
	MovableObject* getHitObject()
	{
		return mHitObject;
	}
	bool hasHits()
	{
		return bHits;
	}



private:
	btDiscreteDynamicsWorld* mCurrentDynamicWolrd;

	MovableObject* mHitObject;

	//True if  we have hits
	bool bHits;
};

#endif