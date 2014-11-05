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
		ENTITY,
		LIGHT,
		CAMERA
	} tTypeEnum;

	std::string getName()
	{
		return mName;
	}
	
	bool isAttachedToNode()
	{
		return mAttachedToNode;
	}

	SceneNode* getAttachedSceneNode()
	{
		return mParentSceneNode;
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

protected:
	std::string mName;
	bool visible;
	bool castShadows;
	bool mAttachedToNode; //is attached to a scenode?
	bool drawAABB;

	tTypeEnum mTypeOfMovableObject;


	SceneManager* mSceneManager;
	SceneNode* mParentSceneNode;
};

#endif