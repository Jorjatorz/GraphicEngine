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

	//Load the mesh
	attachMesh(meshName);
}


Entity::~Entity(void)
{
	//Rigidbody has the same name as entity
	PhysicsManager::getSingletonPtr()->deleteRigidBody(mName);
}

void Entity::process(glm::mat4 perspectiveViewSceneNodeM, glm::mat4 viewMatrix, glm::vec3 parentPos, glm::quat parentOrient)
{
	//If its has a rigidBody (i.e not a light volume) set the transformations (only if kinetic)
	if (mRigidBody != NULL)
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
		//We are not attached to a scenenode -> we dont have a position -> return error vector
		return glm::vec3(-9999999.9);
	}
}
glm::vec3 Entity::getOrientation_Euler()
{
	if (mParentSceneNode != NULL)
	{
		//Transform the quaternion into euler angles
		return glm::eulerAngles(mParentSceneNode->getDerivedOrientation());
	}
	else
	{
		//We are not attached to a scenenode -> we dont have a orientation -> return error vector
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
		//We are not attached to a scenenode -> we dont have a scale -> return error vector
		return glm::vec3(-9999999.9);
	}
}

void Entity::render(glm::mat4 perspectiveViewSceneNodeM, glm::mat4 viewMatrix)
{
	//We render only if we have a mesh to render
	if(meshAttached)
	{
		//Loop through all the submeshes of the mesh and render them
		for(int i = 0; i < mMesh->mMeshComponentsVector.size(); ++i)
		{

			// Apply shader. If we dont have a full entity material use each entity material (created when loading the mesh)
			Shader* materialShader;
			if (mMaterial == NULL)
			{
				materialShader = mMesh->mMeshComponentsVector[i].meshMaterial->getShader();
			}
			else
			{
				materialShader = mMaterial->getShader();
			}
			mSceneManager->bindShader(materialShader);

			//Send all entity uniforms	
			sendEntityUniforms(materialShader, perspectiveViewSceneNodeM, viewMatrix);

			mMesh->bindMeshArray(mMesh->mMeshComponentsVector[i]);
			//Apply the material (i.e. Send material uniforms)
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

		//After rendering the object we render the AABB
		if (drawAABB)
		{
			mSceneManager->bindShader(mSceneManager->createShader("AABBshader", "AABB"));
			mMesh->renderAABB(mModelMatrix, perspectiveViewSceneNodeM);
		}

		//Reactivate Fill mode to avoid problems
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Entity::sendEntityUniforms(Shader* currentShader, glm::mat4 PVNMatrix, glm::mat4 viewMatrix)
{
	glm::mat4 finalMatrix = PVNMatrix * mModelMatrix; //final matrix composed of pers * view * node * model matrix

	//Calculate the normal matrix (Inverse(Transpose(ModelMatrix))); If we dont have scenenode we cant multiply by the node matrix (i.e. deferred light volums)
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

	ResourceManager* mResourceManager = ResourceManager::getSingletonPtr();
	//Load or get the Mesh pointer.
	mMesh = mResourceManager->loadMesh(mMeshName, mMeshName, mSceneManager);

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
	//Creates or return a material with that name
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
	//Will be deleted in the constructor. The rigigdBody uses the same name as the entity
	mRigidBody = PhysicsManager::getSingletonPtr()->createRigidBody(mName, node, this);
}

void Entity::setRigidBodyTransforms(SceneNode* node)
{
	//Only if kinetic. Updates the new transforms in the rigidBody
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

	//Care, inverse bools!
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