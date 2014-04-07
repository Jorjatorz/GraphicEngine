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
}

void SceneNode::setParentSceneNode(SceneNode* newParent)
{
	mParent = newParent;
}

void SceneNode::addChildSceneNode(SceneNode* newChild)
{
	mChildNodes.push_back(newChild); //add the new child
}

void SceneNode::updateFromParent()
{
	getDerivedPosition();
	getDerivedOrientation();
	getDerivedScale();

	if(hasChanged)
	{
		mSceneNodeMatrix = glm::translate(mSceneNodeMatrix, mDerivedPosition);
		mSceneNodeMatrix = glm::rotate(mSceneNodeMatrix, mDerivedOrientation.x, glm::vec3(1.0, 0.0, 0.0));
		mSceneNodeMatrix = glm::rotate(mSceneNodeMatrix, mDerivedOrientation.y, glm::vec3(0.0, 1.0, 0.0));
		mSceneNodeMatrix = glm::rotate(mSceneNodeMatrix, mDerivedOrientation.z, glm::vec3(0.0, 0.0, 1.0));
		mSceneNodeMatrix = glm::scale(mSceneNodeMatrix, mDerivedScale);
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