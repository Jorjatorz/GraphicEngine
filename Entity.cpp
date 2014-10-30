#include "Entity.h"


#include "Mesh.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "SceneNode.h"
#include "Root.h"
#include "Material.h"
#include "RigidBody.h"
#include "CollisionObject.h"
#include "PhysicsManager.h"

Entity::Entity(std::string mNewName, SceneManager* newSceneManager)
{
	//apart from movableobject constructor
	mName = mNewName;
	mModelMatrix = glm::mat4(1.0); //identity
	mMesh = NULL;
	mMeshName = "NULL";
	mMaterial = NULL;
	mSceneManager = newSceneManager;
	meshAttached = false;

	mAffectedByPhysics = true;
	
	wireFrame = false;
	drawAABB = false;

	mTypeOfMovableObject = tTypeEnum::ENTITY;
}

Entity::Entity(std::string mNewName, std::string meshName, SceneManager* newSceneManager)
{
	mName = mNewName;
	mModelMatrix = glm::mat4(1.0); //identity
	mSceneManager = newSceneManager;
	meshAttached = false;
	mMaterial = NULL;
	mRigidBody = NULL;

	mAffectedByPhysics = true;

	mTypeOfMovableObject = tTypeEnum::ENTITY;

	attachMesh(meshName); //load mesh
}


Entity::~Entity(void)
{
	//Rigidbody has the same name as entity
	PhysicsManager::getSingletonPtr()->deleteRigidBody(mName);
}

void Entity::process(glm::mat4 perspectiveViewSceneNodeM, glm::mat4 viewMatrix, glm::vec3 parentPos, glm::quat parentOrient)
{
	//If its a "real" entity (i.e not a light volume)
	if (mParentSceneNode != NULL)
	{
		mRigidBody->setTransforms(mParentSceneNode);
	}

	if (visible)
	{
		render(perspectiveViewSceneNodeM, viewMatrix);
	}
}

glm::vec3 Entity::getPosition()
{
	if (mParentSceneNode != NULL)
	{
		return mParentSceneNode->getDerivedPosition();
	}
	else
	{
		return glm::vec3(-9999999.9);
	}
}
glm::vec3 Entity::getOrientation_Euler()
{
	if (mParentSceneNode != NULL)
	{
		return glm::eulerAngles(mParentSceneNode->getDerivedOrientation());
	}
	else
	{
		return glm::vec3(-9999999.9);
	}
}

glm::vec3 Entity::getScale()
{
	if (mParentSceneNode != NULL)
	{
		return mParentSceneNode->getDerivedScale();
	}
	else
	{
		return glm::vec3(-9999999.9);
	}
}

void Entity::render(glm::mat4 perspectiveViewSceneNodeM, glm::mat4 viewMatrix)
{
	if(meshAttached)
	{

		for(int i = 0; i < mMesh->mMeshComponentsVector.size(); ++i)
		{

			// apply shader. If we dont have a full entity material use each entity material
			Shader* materialShader;
			if (mMaterial == NULL)
			{
				materialShader = mMesh->mMeshComponentsVector[i].meshMaterial->getShader();
				mSceneManager->bindShader(materialShader);
			}
			else
			{
				materialShader = mMaterial->getShader();
				mSceneManager->bindShader(materialShader);
			}

			//Send uniforms	
			sendEntityUniforms(materialShader, perspectiveViewSceneNodeM, viewMatrix);

			//render
			mMesh->bindMeshArray(mMesh->mMeshComponentsVector[i]);

			//Set the material for each mesh (can be optimice the else)
			if (mMaterial == NULL)
			{
				mMesh->mMeshComponentsVector[i].meshMaterial->applyMaterial();
			}
			else
			{
				mMaterial->applyMaterial();
			}

			if (wireFrame)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			glDrawElements(GL_TRIANGLES, mMesh->mMeshComponentsVector[i].mIndexVector.size(), GL_UNSIGNED_INT, 0);

			mMesh->unbindMeshArray();

		}

		//After rendering the object
		if (drawAABB)
		{
			mSceneManager->bindShader(mSceneManager->createShader("AABBshader", "AABB"));
			mMesh->renderAABB(mModelMatrix, perspectiveViewSceneNodeM);
		}
	}
}

void Entity::sendEntityUniforms(Shader* currentShader, glm::mat4 PVNMatrix, glm::mat4 viewMatrix)
{
	glm::mat4 finalMatrix = PVNMatrix * mModelMatrix; //final matrix composed of pers * view * node * model matrix

	//IF we dont have node we cant multiply by the node matrix (i.e. deferred light volums
	glm::mat4 normalM;
	if (mParentSceneNode != NULL)
	{
		normalM = glm::inverseTranspose(mParentSceneNode->getSceneNodeMatrix() * mModelMatrix);
	}
	else
	{
		normalM = glm::inverseTranspose(mModelMatrix);
	}
	currentShader->UniformMatrix("MVP", finalMatrix);
	currentShader->UniformMatrix("projectionM", mSceneManager->getProjectionMatrix());
	currentShader->UniformMatrix("viewM", viewMatrix);
	currentShader->UniformMatrix("normalM", normalM);
	currentShader->UniformMatrix("modelM", mModelMatrix);
}

void Entity::attachMesh(std::string meshName)
{
	mMeshName = meshName;

	ResourceManager* mResourceManager = ResourceManager::getSingletonPtr(); //resourcemanager pointer
	mMesh = mResourceManager->loadMesh(mMeshName, mMeshName, mSceneManager); //allocate new mesh

	mModelMatrix = mMesh->meshMatrix;

	meshAttached = true;
}

void Entity::deAttachMesh()
{
	mMeshName = "NULL";
	mMesh = NULL;
	meshAttached = false;
}

void Entity::attachMaterial(std::string materialName)
{
	//Creates o return a material with that name
	mMaterial = mSceneManager->createMaterial(materialName, materialName);
}

void Entity::setWireFrame(bool wire)
{
	wireFrame = wire;
}

void Entity::setModelMatrix(glm::mat4 matrix)
{
	mModelMatrix = matrix;
}

void Entity::makeRigidBody(SceneNode* node)
{
	mRigidBody = PhysicsManager::getSingletonPtr()->createRigidBody(mName, node, this);
}

void Entity::setRigidBodyTransforms(SceneNode* node)
{
	mRigidBody->setTransforms(node);
}

real Entity::getMass()
{
	return mRigidBody->getMass();
}

void Entity::setMass(real mass, bool setStatic)
{
	mRigidBody->setMass(mass, setStatic);
}

void Entity::setPhysicsOn(bool mod)
{
	mAffectedByPhysics = mod;

	if (mAffectedByPhysics)
	{
		mRigidBody->makeRigidBodyWithNoCollisions(false);
	}
	else
	{
		mRigidBody->makeRigidBodyWithNoCollisions(true);
	}
}

void Entity::setRayCastReturnPointer(void *returnPointer)
{
	mRigidBody->setRayCastReturnPointer(returnPointer);
}

void Entity::setColor(glm::vec3 col)
{
	if (mMaterial != NULL)
	{
		mMaterial->setBaseColorV(col);
	}
}