#ifndef MOVABLEOBJECT_H
#define MOVABLEOBJECT_H

#include "Definitions.h"

#include <string>

class MovableObject
{
public:
	MovableObject(void);
	~MovableObject(void);

	//type enumeration for detecting what inherit class it is
	typedef enum
	{
		Entity,
		Light,
		Camera
	} tTypeEnum;

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

	tTypeEnum getType()
	{
		return mTypeOfMovableObject;
	}

	void setVisible(bool vis)
	{
		visible = vis;
	}

	void showAABB(bool show);
	bool isAABBshowed()
	{
		return drawAABB;
	}

	void setAttached(bool att);
	void setAttached(bool att, SceneNode* newNode);

	virtual void process(glm::mat4 perspectiveViewSceneNodeM, glm::mat4 viewMatrix, glm::vec3 parentPos, glm::quat parentOrient) = 0;

	SceneManager* mSceneManager;
	SceneNode* mParentSceneNode;

protected:
	std::string mName;
	bool visible;
	bool castShadows;
	bool mAttachedToNode; //is attached to a scenode?
	bool drawAABB;

	tTypeEnum mTypeOfMovableObject;
};

#endif