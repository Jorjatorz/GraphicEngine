#include "MovableObject.h"


MovableObject::MovableObject(void)
{
	mName = "Unknown";
	mSceneManager = NULL;
	mParentSceneNode = NULL;
	visible = true;
	castShadows = false;
	mAttachedToNode = false;
}


MovableObject::~MovableObject(void)
{
}

void MovableObject::setAttached(bool att)
{
	mAttachedToNode = att;
}

void MovableObject::setAttached(bool att, SceneNode* newNode)
{
	mAttachedToNode = att;
	mParentSceneNode = newNode;
}
