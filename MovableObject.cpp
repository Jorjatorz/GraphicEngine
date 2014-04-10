#include "MovableObject.h"


MovableObject::MovableObject(void)
{
	mName = "Unknown";
	mSceneManager = NULL;
	mParentSceneNode = NULL;
	visible = true;
	castShadows = false;
	mAttached = false;
}


MovableObject::~MovableObject(void)
{
}

void MovableObject::setAttached(bool att)
{
	mAttached = att;
}

void MovableObject::setAttached(bool att, SceneNode* newNode)
{
	mAttached = att;
	mParentSceneNode = newNode;
}
