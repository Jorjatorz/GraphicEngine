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

private:
	//scenenode model matrix (not derived from parent)
	glm::mat4 mSceneNodeMatrix;
	//true if the node has changed
	bool hasChanged;
	//parent
	SceneNode* mParent;

	//object attachet map
	typedef std::map<std::string, MovableObject*> tObjectsMaps;
	tObjectsMaps mObjectsMap;
	//child vector scenenode
	std::vector<SceneNode*> mChildNodes;
};

#endif