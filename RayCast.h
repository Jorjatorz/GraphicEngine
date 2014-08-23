#ifndef RAYCAST_H
#define RAYCAST_H

#include "Definitions.h"

class RayCast
{
public:
	RayCast();
	~RayCast();

	void executeRaySelectionCast(glm::vec3 start, glm::vec3 end); //End can be used as direction multiplaying the direction by a high number

	bool hasHits;

private:
	btDiscreteDynamicsWorld* mCurrentDynamicWolrd;

	Entity* mHitObject;
};

#endif