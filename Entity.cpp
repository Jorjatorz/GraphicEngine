#include "Entity.h"


#include "Mesh.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "SceneNode.h"
#include "Root.h"
#include "Material.h"

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
	
	wireFrame = false;
	drawAABB = false;

	mTypeOfMovableObject = tTypeEnum::Entity;
}

Entity::Entity(std::string mNewName, std::string meshName, SceneManager* newSceneManager)
{
	mName = mNewName;
	mModelMatrix = glm::mat4(1.0); //identity
	mSceneManager = newSceneManager;
	meshAttached = false;
	mMaterial = NULL; 

	mTypeOfMovableObject = tTypeEnum::Entity;

	attachMesh(meshName); //load mesh
}


Entity::~Entity(void)
{
}

void Entity::process(glm::mat4 perspectiveViewSceneNodeM, glm::mat4 viewMatrix, glm::vec3 parentPos, glm::quat parentOrient)
{
	render(perspectiveViewSceneNodeM, viewMatrix);
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
	currentShader->UniformMatrix("projectionM", mSceneManager->getPerspectiveMatrix());
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

void Entity::showAABB(bool show)
{
	drawAABB = show;
}