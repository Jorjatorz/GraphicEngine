#include "SceneNode.h"


SceneNode::SceneNode(void)
{
	mParent = NULL;
	mPosition = mOrientation = glm::vec3(0.0);
	mScale = glm::vec3(1.0);
	hasChanged = false;
}


SceneNode::~SceneNode(void)
{
	//delete all childs
	//remove node from parent
}

void SceneNode::setParentSceneNode(SceneNode* newParent)
{
	mParent = newParent;
}

void SceneNode::addChildSceneNode(std::string name, SceneNode* newChild)
{
	mChildNodes.insert(std::pair<std::string, SceneNode*>(name, newChild));
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