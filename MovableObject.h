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
	
	bool isAttachedToNode()
	{
		return mAttachedToNode;
	}

	bool isVisible()
	{
		return visible;
	}

	void setVisible(bool vis)
	{
		visible = vis;
	}

	void setAttached(bool att);
	void setAttached(bool att, SceneNode* newNode);

	virtual void render(glm::mat4 perspectiveViewM) = 0;

protected:
	std::string mName;
	SceneManager* mSceneManager;
	SceneNode* mParentSceneNode;
	bool visible;
	bool castShadows;
	bool mAttachedToNode; //is attached to a scenode?
};

#endif