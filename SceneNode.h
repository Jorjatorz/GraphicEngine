#ifndef SCENENODE_H
#define SCENENODE_H

#include "Definitions.h"

#include <map>
#include <vector>

class SceneNode
{
public:
	SceneNode(void);
	~SceneNode(void);

private:
	//scenenode model matrix
	glm::mat4 mSceneNodeMatrix;
	//true if the node has changed
	bool hasChanged;
	//parent
	SceneNode* mParent;

	//object attachet map

	//child vector scenenode
	std::vector<SceneNode*> mChildNodes;
};

#endif