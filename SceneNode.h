#ifndef SCENENODE_H
#define SCENENODE_H

#include "Definitions.h"

#include <iostream>
#include <string>
#include <map>
#include <vector>

class SceneNode
{
public:
	SceneNode(void);
	SceneNode(std::string sceneNodeName, SceneManager* newSceneManager);
	SceneNode(std::string sceneNodeName, SceneNode* nodeParent, SceneManager* newSceneManager);
	~SceneNode(void);

	//process all attached movableobjects to the scenenode (just for the root scenenode)
	void processRootSceneNode();

	//sets the new paren to the sceneNode
	void setParentSceneNode(SceneNode* newParent);
	//adds a new child to the sceneNode
	SceneNode* createChildSceneNode(std::string name);
	SceneNode* createChildSceneNode(std::string name, glm::vec3 newPosition);
	SceneNode* createChildSceneNode(std::string name, SceneNode* newChild);
	//deletes childrens
	void deleteChildrenNode(SceneNode* mNode);
	void deleteChildrenNode(std::string childName);
	void deleteAllChilds();
	//attach a movableobjecto to the scenenode
	void attachObject(MovableObject* mNewObject);
	//deatach an object or all objects
	void detachObject(std::string objName);
	void detachObject(MovableObject* obj);
	void detachAllObjects();


	//transformations functions
	void translate(glm::vec3 trans);
	void rotate(glm::vec3 rot);
	void lookAt(glm::vec3 destination);
	void setPosition(glm::vec3 newPos);
	void setOrientation(glm::vec3 newOrientation);
	void setScale(glm::vec3 newScale);

	//returns
	glm::mat4 getSceneNodeMatrix()
	{
		return mSceneNodeMatrix;
	}

	glm::vec3 getPosition()
	{
		return mPosition;
	}
	glm::vec3 getOrientation()
	{
		return mOrientation;
	}
	glm::vec3 getScale()
	{
		return mScale;
	}
	glm::vec3 getDerivedPosition()
	{
		return mDerivedPosition;
	}
	glm::vec3 getDerivedOrientation()
	{
		return mDerivedOrientation;
	}
	glm::vec3 getDerivedScale()
	{
		return mDerivedScale;
	}

private:
	//scenenode model matrix (derived from parent)
	glm::mat4 mSceneNodeMatrix;
	//scenenode position, orientation and scale
	glm::vec3 mPosition, mOrientation, mScale;
	//derived scenenode position, orientation and scale
	glm::vec3 mDerivedPosition, mDerivedOrientation, mDerivedScale;

	//true if the node has changed
	bool hasChanged;
	//parent
	SceneNode* mParent;

	//object attachet map
	typedef std::map<std::string, MovableObject*> tObjectsMaps;
	tObjectsMaps mObjectsMap;
	//child vector scenenode
	typedef std::map<std::string, SceneNode*> tChildsNodesMap;
	tChildsNodesMap mChildNodes;

	//scenenode name
	std::string mName;
	//pointer to the current sceneManager
	SceneManager* mSceneManager;

	//Functions
	void processChilds(glm::mat4 perspectiveViewM); //go trhought out all his childs and process his objects
	void processObjects(glm::mat4 perspectiveViewM); //rederall the objects attached to the scenenode

	void processDerivedPosition(); //modify derivedPos. the parent position * mPosition
	void processDerivedOrientation(); //modify derivedOri the parent orientation * mOrientation
	void processDerivedScale();	//modify derivedScale the parent scale * mScane

	void updateFromParent(); //process the derived traslations and update the scenenodeMatrix
	void updateChildrens(); //update the childrens using the current derived translations
};

#endif