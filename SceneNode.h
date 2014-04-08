#ifndef SCENENODE_H
#define SCENENODE_H

#include "Definitions.h"

#include <string>
#include <map>
#include <vector>

class SceneNode
{
public:
	SceneNode(void);
	~SceneNode(void);

	//sets the new paren to the sceneNode
	void setParentSceneNode(SceneNode* newParent);
	//adds a new child to the sceneNode
	void addChildSceneNode(std::string name, SceneNode* newChild);
	//get the new derived traslations and update the scenenodeMatrix
	void updateFromParent();
	//update the childrens using the current derived position
	void updateChildrens();

	//modify derivedPos. the parent position * mPosition
	void getDerivedPosition();
	//modify derivedOri the parent orientation * mOrientation
	void getDerivedOrientation();
	//modify derivedScale the parent scale * mScane
	void getDerivedScale();

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
};

#endif