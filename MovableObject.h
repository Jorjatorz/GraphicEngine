#ifndef MOVABLEOBJECT_H
#define MOVABLEOBJECT_H

#include "Definitions.h"

#include <string>

class MovableObject
{
public:
	MovableObject(void);
	~MovableObject(void);

	std::string getName()
	{
		return mName;
	}
	
	bool isAttached()
	{
		return mAttached;
	}

	void setAttached(bool att);
	void setAttached(bool att, SceneNode* newNode);

protected:
	std::string mName;
	SceneManager* mSceneManager;
	SceneNode* mParentSceneNode;
	bool visible;
	bool castShadows;
	bool mAttached; //is attached to a scenode?
};

#endif