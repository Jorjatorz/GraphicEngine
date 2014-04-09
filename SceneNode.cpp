#include "SceneNode.h"


SceneNode::SceneNode(void)
{
	mParent = NULL;
	mPosition = mOrientation = glm::vec3(0.0);
	mScale = glm::vec3(1.0);
	mSceneNodeMatrix = glm::mat4(1.0);
	hasChanged = true;
	mName = "Unknown";
}

SceneNode::SceneNode(std::string sceneNodeName)
{
	mParent = NULL;
	mPosition = mOrientation = glm::vec3(0.0);
	mScale = glm::vec3(1.0);
	mSceneNodeMatrix = glm::mat4(1.0);
	hasChanged = true;
	mName = sceneNodeName;
}

SceneNode::SceneNode(std::string sceneNodeName, SceneNode* nodeParent)
{
	mParent = nodeParent;
	mPosition = mOrientation = glm::vec3(0.0);
	mScale = glm::vec3(1.0);
	mSceneNodeMatrix = glm::mat4(1.0);
	hasChanged = true;
	mName = sceneNodeName;
}

SceneNode::~SceneNode(void)
{
	//delete all childs
	deleteAllChilds();
	//remove node from parent
}

void SceneNode::setParentSceneNode(SceneNode* newParent)
{
	mParent = newParent;
}

SceneNode* SceneNode::addChildSceneNode(std::string name)
{
	SceneNode* newSceneNode = new SceneNode(name, this);

	mChildNodes.insert(std::pair<std::string, SceneNode*>(name, newSceneNode));

	return newSceneNode;
}

SceneNode* SceneNode::addChildSceneNode(std::string name, SceneNode* newChild)
{
	mChildNodes.insert(std::pair<std::string, SceneNode*>(name, newChild));

	return newChild;
}

void SceneNode::deleteChildrenNode(SceneNode* mNode)
{
	deleteChildrenNode(mNode->mName);
}
void SceneNode::deleteChildrenNode(std::string childName)
{
	tChildsNodesMap::iterator it;

	for(it = mChildNodes.begin(); it != mChildNodes.end(); ++it)
	{
		if(childName == it->first)
		{
			//delete the scenenode (the destructor will also delete his childs)
			delete it->second;
			mChildNodes.erase(it);

			break;
		}
	}
}
void SceneNode::deleteAllChilds()
{
	tChildsNodesMap::iterator it;

	for(it = mChildNodes.begin(); it != mChildNodes.end(); ++it)
	{
		delete it->second;
	}

	mChildNodes.clear();
}

void SceneNode::updateFromParent()
{
	getDerivedPosition();
	getDerivedOrientation();
	getDerivedScale();

	//if the node position has changed or the derived positon (thus the parent) has changed modify the sceneNodeMatrix
	if(hasChanged)
	{
		//update childs
		updateChildrens();
		//update matrix
		mSceneNodeMatrix = glm::translate(mSceneNodeMatrix, mDerivedPosition);
		mSceneNodeMatrix = glm::rotate(mSceneNodeMatrix, mDerivedOrientation.x, glm::vec3(1.0, 0.0, 0.0));
		mSceneNodeMatrix = glm::rotate(mSceneNodeMatrix, mDerivedOrientation.y, glm::vec3(0.0, 1.0, 0.0));
		mSceneNodeMatrix = glm::rotate(mSceneNodeMatrix, mDerivedOrientation.z, glm::vec3(0.0, 0.0, 1.0));
		mSceneNodeMatrix = glm::scale(mSceneNodeMatrix, mDerivedScale);
	}

}

void SceneNode::updateChildrens()
{
	tChildsNodesMap::iterator it;

	for(it = mChildNodes.begin(); it != mChildNodes.end(); ++it)
	{
		it->second->updateFromParent();
	}
}

void SceneNode::getDerivedPosition()
{
	//if we have parent
	if(mParent != NULL)
	{
		if(mParent->hasChanged)
		{
			mDerivedPosition = mParent->mDerivedPosition * mPosition;

			hasChanged = true;
		}
	}

	mDerivedPosition = mPosition;
}
void SceneNode::getDerivedOrientation()
{
		//if we have parent
	if(mParent != NULL)
	{
		if(mParent->hasChanged)
		{
			mDerivedOrientation = mParent->mDerivedOrientation * mOrientation;
			
			hasChanged = true;
		}
	}

	mDerivedOrientation = mOrientation;
}
void SceneNode::getDerivedScale()
{
		//if we have parent
	if(mParent != NULL)
	{
		if(mParent->hasChanged)
		{
			mDerivedScale = mParent->mDerivedScale * mScale;

			hasChanged = true;
		}
	}

	mDerivedScale = mDerivedScale;
}