#include "SceneNode.h"

#include "MovableObject.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Entity.h"
#include "RigidBody.h"

SceneNode::SceneNode(std::string sceneNodeName, SceneNode* nodeParent, SceneManager* newSceneManager)
{
	mParent = nodeParent;
	mPosition = glm::vec3(0.0);
	mOrientation = glm::quat();
	mScale = glm::vec3(1.0);
	mDerivedPosition = glm::vec3(0.0);
	mDerivedOrientation = glm::quat();
	mDerivedScale = glm::vec3(1.0);
	mSceneNodeMatrix = glm::mat4(1.0);
	hasChanged = true;
	mName = sceneNodeName;
	mSceneManager = newSceneManager;

	updateFromParent();
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
	tChildsNodesMap::iterator it = mChildNodes.find(name);;

	if(it != mChildNodes.end())
	{
		#ifdef DEBUG_MESSAGES
		std::cout << name << " SceneNode already exists, returning it" << std::endl;
		#endif
		//if already exists retuern it
		return it->second;
	}
	//else
	SceneNode* newSceneNode = new SceneNode(name, this, mSceneManager);

	mChildNodes.insert(std::pair<std::string, SceneNode*>(name, newSceneNode));

	return newSceneNode;
}

SceneNode* SceneNode::createChildSceneNode(std::string name, SceneNode* newChild)
{
	tChildsNodesMap::iterator it = mChildNodes.find(name);

	if(it != mChildNodes.end())
	{
		#ifdef DEBUG_MESSAGES
		std::cout << name << " SceneNode already exists, returning it" << std::endl;
		#endif
		//Change the current node with the new one and return it
		it->second = newChild;
		return it->second;
	}
	//else
	mChildNodes.insert(std::pair<std::string, SceneNode*>(name, newChild));

	return newChild;
}

SceneNode* SceneNode::createChildSceneNode(std::string name, glm::vec3 newPosition)
{
	tChildsNodesMap::iterator it = mChildNodes.find(name);

	if(it != mChildNodes.end())
	{
		#ifdef DEBUG_MESSAGES
		std::cout << name << " SceneNode already exists, returning it" << std::endl;
		#endif
		//if already exists retuern it
		return it->second;
	}
	//else
	SceneNode* newSceneNode = new SceneNode(name, this, mSceneManager);

	newSceneNode->setPosition(newPosition);

	mChildNodes.insert(std::pair<std::string, SceneNode*>(name, newSceneNode));

	return newSceneNode;
}

void SceneNode::deleteChildrenNode(SceneNode* mNode)
{
	deleteChildrenNode(mNode->mName);
}
void SceneNode::deleteChildrenNode(std::string childName)
{
	tChildsNodesMap::iterator it = mChildNodes.find(childName);

	if(it != mChildNodes.end())
	{
		//if it exists delete it
		delete it->second;
		mChildNodes.erase(it);
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
		//its already attached to this node, set new tranforms just for kinetic objects
		if (mNewObject->getType() == MovableObject::ENTITY)
		{
			static_cast<Entity*>(mNewObject)->getRigidBody()->setTransforms(this);
		}
	}
	else if(mNewObject->isAttachedToNode())
	{
		//Detach object from old node
		mNewObject->mParentSceneNode->detachObject(mNewObject);

		//Attach object to new node
		mObjectsMap.insert(std::pair<std::string, MovableObject*>(mNewObject->getName(), mNewObject));
		mNewObject->setAttached(true, this);

		//Update the transforms with the new node (Just for kinetic objects)
		if (mNewObject->getType() == MovableObject::ENTITY)
		{
			static_cast<Entity*>(mNewObject)->getRigidBody()->setTransforms(this);
		}
	}
	else
	{
		mObjectsMap.insert(std::pair<std::string, MovableObject*>(mNewObject->getName(), mNewObject));
		mNewObject->setAttached(true, this);

		//First time, create collision object
		if (mNewObject->getType() == MovableObject::ENTITY)
		{
			static_cast<Entity*>(mNewObject)->makeRigidBody(this);
		}
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
	detachObject(obj->getName());
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
	if(hasChanged || mParent->hasChanged)
	{
		mSceneNodeMatrix = glm::mat4(1.0); //set identity
		//update childs
		updateChildrens();
		//update matrix
		mSceneNodeMatrix = glm::translate(mSceneNodeMatrix, mDerivedPosition);
		mSceneNodeMatrix = mSceneNodeMatrix * glm::toMat4(mOrientation);
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
			mDerivedPosition = mParent->mDerivedPosition + mPosition;
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
			mDerivedOrientation = mParent->mDerivedOrientation + mOrientation;
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
			mDerivedScale = mParent->mDerivedScale * mScale;
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
	glm::mat4 perspectiveViewM = mSceneManager->getProjectionMatrix() * mSceneManager->getViewMatrix();

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

void SceneNode::rotate(glm::vec3 axis, real angle)
{
	/*glm::quat q;
	q = glm::angleAxis(angle, axis); //Suppose angle is in degrees
	glm::normalize(q); //Normalize to avoid drift

	mOrientation = q; //Order is */

	glm::mat4 rotationM;
	if (axis != glm::vec3(0.0)) //Just in case
	{
		rotationM = glm::rotate(glm::mat4(1.0), angle, axis);
	}

	mOrientation = glm::quat(rotationM);

	hasChanged = true;
}

void SceneNode::scale(glm::vec3 axis)
{
	mScale += axis;

	hasChanged = true;
}

void SceneNode::lookAt(glm::vec3 lookAtPoint, glm::vec3 objectFacing)
{
	/*glm::vec3 lookVector = lookAtPoint;
	assert(lookVector != mPosition);
	
	glm::vec3 direction = glm::normalize(lookVector - mPosition);
	float dot = glm::dot(glm::vec3(0.0, 0.0, 1.0), direction);

	glm::quat rotation;
	if (fabs(dot - (-1.0f)) < 0.000001f) {
		rotation = glm::angleAxis((real)glm::degrees(M_PI), glm::vec3(0.0, 1.0, 0.0));
		mOrientation = rotation;
		return;
	}
	else if (fabs(dot - (1.0f)) < 0.000001f) {
		rotation = glm::quat();
		mOrientation = rotation;
		return;
	}

	float angle = glm::degrees(acosf(dot));

	glm::vec3 cross = glm::normalize(glm::cross(glm::vec3(0.0, 0.0, 1.0), direction));
	rotation = glm::normalize(glm::angleAxis(angle, cross));

	mOrientation = mOrientation * rotation;*/

	glm::vec3 targetVec = glm::normalize(lookAtPoint - mPosition);
	glm::vec3 mDir = objectFacing;
	glm::vec3 mAxis = glm::cross(targetVec, mDir);
	real angle = glm::acos(glm::dot(targetVec, mDir));

	if (mAxis != glm::vec3(0.0))
	{
		rotate(mAxis, -glm::degrees(angle));
	}

}
void SceneNode::setPosition(glm::vec3 newPos)
{
	mPosition = newPos;

	hasChanged = true;
}
void SceneNode::setOrientation(glm::quat newOrientation)
{
	mOrientation = newOrientation;

	hasChanged = true;
}
void SceneNode::setScale(glm::vec3 newScale)
{
	mScale = newScale;

	hasChanged = true;
}