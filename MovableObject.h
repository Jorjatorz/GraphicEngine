#ifndef MOVABLEOBJECT_H
#define MOVABLEOBJECT_H

#include "Definitions.h"

#include <string>

class MovableObject
{
public:
	MovableObject(void);
	~MovableObject(void);

protected:
	std::string mName;
	SceneManager* mSceneManager;
	SceneNode* mParentSceneNode;
	bool visible;
	bool castShadows;
};

#endif