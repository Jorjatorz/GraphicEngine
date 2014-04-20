#include "SceneNode.h"

#include "MovableObject.h"
#include "SceneManager.h"

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
	if(mNewObject->isAttached())
	{
		#ifdef DEBUG_MESSAGE
		std::cout << mNewObject->getName() << " already attached" << std::endl;
		#endif
	}
	else
	{
		mObjectsMap.insert(std::pair<std::string, MovableObject*>(mNewObject->getName(), mNewObject));
		mNewObject->setAttached(true, this);
	}
}

void SceneNode::detachObject(std::string objName)
{
	tObjectsMaps::iterator it;

	for(it = mObjectsMap.begin(); it != mObjectsMap.end(); ++it)
	{
		if(it->second->getName() == objName)
		{
			it->second->setAttached(false, NULL);
			mObjectsMap.erase(it);
			break;
		}
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
	getDerivedPosition();
	getDerivedOrientation();
	getDerivedScale();

	//if the node position has changed or the derived positon (thus the parent) has changed modify the sceneNodeMatrix
	if(hasChanged)
	{
		mSceneNodeMatrix = glm::mat4(1.0); //identity
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

void SceneNode::getDerivedPosition()
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
void SceneNode::getDerivedOrientation()
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
	else
	{
		mDerivedScale = mScale;
	}
}

void SceneNode::processRootSceneNode()
{
	//calculate perspective and viewmatrix multiplyed matrix
	glm::mat4 perspectiveViewM = mSceneManager->getPerspectiveMatrix();

	//update positions
	updateChildrens();

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
		it->second->render(finalMatrix);
	}
}

void SceneNode::translate(glm::vec3 trans)
{
	mPosition += trans;

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