#include "SceneNode.h"

#include "MovableObject.h"
#include "SceneManager.h"
#include "Camera.h"

SceneNode::SceneNode(void)
{
	mParent = NULL;
	mPosition = mOrientation = glm::vec3(0.0);
	mScale = glm::vec3(1.0);
	mDerivedPosition = mDerivedOrientation = glm::vec3(0.0);
	mDerivedScale = glm::vec3(1.0);
	mSceneNodeMatrix = glm::mat4(1.0);
	hasChanged = true;
	mName = "Unknown";
	mSceneManager = NULL;
}

SceneNode::SceneNode(std::string sceneNodeName, SceneManager* newSceneManager)
{
	mParent = NULL;
	mPosition = mOrientation = glm::vec3(0.0);
	mScale = glm::vec3(1.0);
	mDerivedPosition = mDerivedOrientation = glm::vec3(0.0);
	mDerivedScale = glm::vec3(1.0);
	mSceneNodeMatrix = glm::mat4(1.0);
	hasChanged = true;
	mName = sceneNodeName;
	mSceneManager = newSceneManager;
}

SceneNode::SceneNode(std::string sceneNodeName, SceneNode* nodeParent, SceneManager* newSceneManager)
{
	mParent = nodeParent;
	mPosition = mOrientation = glm::vec3(0.0);
	mScale = glm::vec3(1.0);
	mDerivedPosition = mDerivedOrientation = glm::vec3(0.0);
	mDerivedScale = glm::vec3(1.0);
	mSceneNodeMatrix = glm::mat4(1.0);
	hasChanged = true;
	mName = sceneNodeName;
	mSceneManager = newSceneManager;
}

SceneNode::~SceneNode(void)
{
	//delete all childs
	deleteAllChilds();
	//detach all objects
	detachAllObjects();
	//remove node from parent
	mParent->deleteChildrenNode(this);
}

void SceneNode::setParentSceneNode(SceneNode* newParent)
{
	mParent = newParent;
}

SceneNode* SceneNode::createChildSceneNode(std::string name)
{
	tChildsNodesMap::iterator it;

	for(it = mChildNodes.begin(); it != mChildNodes.end(); ++it)
	{
		if(name == it->first)
		{
			#ifdef DEBUG_MESSAGES
			std::cout << name << " SceneNode already exists, returning it" << std::endl;
			#endif
			//if already exists retuern it
			return it->second;
		}
	}
	//else
	SceneNode* newSceneNode = new SceneNode(name, this, mSceneManager);

	mChildNodes.insert(std::pair<std::string, SceneNode*>(name, newSceneNode));

	return newSceneNode;
}

SceneNode* SceneNode::createChildSceneNode(std::string name, SceneNode* newChild)
{
	tChildsNodesMap::iterator it;

	for(it = mChildNodes.begin(); it != mChildNodes.end(); ++it)
	{
		if(name == it->first)
		{
			#ifdef DEBUG_MESSAGES
			std::cout << name << " SceneNode already exists, inserting new sceneNode in it" << std::endl;
			#endif
			//if already exists retuern it
			it->second = newChild;

			return it->second;
		}
	}
	//else
	mChildNodes.insert(std::pair<std::string, SceneNode*>(name, newChild));

	return newChild;
}

SceneNode* SceneNode::createChildSceneNode(std::string name, glm::vec3 newPosition)
{
	tChildsNodesMap::iterator it;

	for(it = mChildNodes.begin(); it != mChildNodes.end(); ++it)
	{
		if(name == it->first)
		{
			#ifdef DEBUG_MESSAGES
			std::cout << name << " SceneNode already exists, returning it" << std::endl;
			#endif
			//if already exists retuern it
			return it->second;
		}
	}
	//else
	SceneNode* newSceneNode = new SceneNode(name, this, mSceneManager);

	newSceneNode->mPosition = newPosition;

	mChildNodes.insert(std::pair<std::string, SceneNode*>(name, newSceneNode));

	return newSceneNode;
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

void SceneNode::attachObject(MovableObject* mNewObject)
{
	//if the object is also attached
	if(mNewObject->mParentSceneNode == this)
	{
		//its already attached to this node, dont do nothing
	}
	else if(mNewObject->isAttachedToNode())
	{
		//Detach object from old node
		mNewObject->mParentSceneNode->detachObject(mNewObject);

		//Attach object to new node
		mObjectsMap.insert(std::pair<std::string, MovableObject*>(mNewObject->getName(), mNewObject));
		mNewObject->setAttached(true, this);
	}
	else
	{
		mObjectsMap.insert(std::pair<std::string, MovableObject*>(mNewObject->getName(), mNewObject));
		mNewObject->setAttached(true, this);
	}
}

void SceneNode::detachObject(std::string objName)
{
	tObjectsMaps::iterator it = mObjectsMap.find(objName);

	if(it != mObjectsMap.end())
	{
		it->second->setAttached(false, NULL);
		mObjectsMap.erase(it);
	}
}

void SceneNode::detachObject(MovableObject* obj)
{
	std::string objName = obj->getName();

	detachObject(objName);
}
void SceneNode::detachAllObjects()
{
	mObjectsMap.clear();
}

void SceneNode::updateFromParent()
{
	processDerivedPosition();
	processDerivedOrientation();
	processDerivedScale();

	//if the node position has changed or the derived positon (thus the parent) has changed modify the sceneNodeMatrix
	if(hasChanged)
	{
		mSceneNodeMatrix = glm::mat4(1.0); //set identity
		//update childs
		updateChildrens();
		//update matrix
		mSceneNodeMatrix = glm::translate(mSceneNodeMatrix, mDerivedPosition);
		mSceneNodeMatrix = glm::rotate(mSceneNodeMatrix, mDerivedOrientation.x, glm::vec3(1.0, 0.0, 0.0));
		mSceneNodeMatrix = glm::rotate(mSceneNodeMatrix, mDerivedOrientation.y, glm::vec3(0.0, 1.0, 0.0));
		mSceneNodeMatrix = glm::rotate(mSceneNodeMatrix, mDerivedOrientation.z, glm::vec3(0.0, 0.0, 1.0));
		mSceneNodeMatrix = glm::scale(mSceneNodeMatrix, mDerivedScale);
	}

	hasChanged = false;

}

void SceneNode::updateChildrens()
{
	tChildsNodesMap::iterator it;

	for(it = mChildNodes.begin(); it != mChildNodes.end(); ++it)
	{
		it->second->updateFromParent();
	}
}

void SceneNode::processDerivedPosition()
{
	//if we have parent
	if(mParent != NULL)
	{
		if(mParent->hasChanged)
		{
			mDerivedPosition = mParent->mDerivedPosition + mPosition;

			hasChanged = true;
		}
	}
	else
	{
		mDerivedPosition = mPosition;
	}
}
void SceneNode::processDerivedOrientation()
{
		//if we have parent
	if(mParent != NULL)
	{
		if(mParent->hasChanged)
		{
			mDerivedOrientation = mParent->mDerivedOrientation + mOrientation;
			
			hasChanged = true;
		}
	}
	else
	{
		mDerivedOrientation = mOrientation;
	}
}
void SceneNode::processDerivedScale()
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
	else
	{
		mDerivedScale = mScale;
	}
}

void SceneNode::processRootSceneNode()
{
	//update positions
	updateChildrens();

	//Update current camera view matrix
	mSceneManager->processViewMatrix();

	//calculate perspective and viewmatrix multiplyed matrix
	glm::mat4 perspectiveViewM = mSceneManager->getPerspectiveMatrix() * mSceneManager->getViewMatrix();

	//go through all the childs
	processChilds(perspectiveViewM);
}

void SceneNode::processChilds(glm::mat4 perspectiveViewM)
{
	//process all objects
	tChildsNodesMap::iterator it;

	for(it = mChildNodes.begin(); it != mChildNodes.end(); ++it)
	{
		//process the objects attached to this node
		it->second->processObjects(perspectiveViewM);
		//process the childs attached to this node
		it->second->processChilds(perspectiveViewM);
	}
}

void SceneNode::processObjects(glm::mat4 perspectiveViewM)
{
	tObjectsMaps::iterator it;

	//we calculate the new matrix (perspective + view + sceneNodeMAtrix)
	glm::mat4 finalMatrix = perspectiveViewM * mSceneNodeMatrix;

	for(it = mObjectsMap.begin(); it != mObjectsMap.end(); ++it)
	{
		//We check if we are updating the current camera (we have already updat eit so we dont want to update it again)
		if((it->second->isVisible()) && (it->second->getName() != mSceneManager->getCurrentCamera()->getName()))
		{
			it->second->process(finalMatrix, mSceneManager->getViewMatrix(), mDerivedPosition, mOrientation);
		}
	}
}

void SceneNode::translate(glm::vec3 trans)
{
	mPosition += trans * mSceneManager->mDeltaTime;

	hasChanged = true;
}

void SceneNode::rotate(glm::vec3 rot)
{
	mOrientation += rot * mSceneManager->mDeltaTime;

	hasChanged = true;
}
void SceneNode::setPosition(glm::vec3 newPos)
{
	mPosition = newPos;

	hasChanged = true;
}
void SceneNode::setOrientation(glm::vec3 newOrientation)
{
	mOrientation = newOrientation;

	hasChanged = true;
}
void SceneNode::setScale(glm::vec3 newScale)
{
	mScale = newScale;

	hasChanged = true;
}