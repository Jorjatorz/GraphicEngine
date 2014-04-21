#include "Entity.h"


#include "Mesh.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "SceneNode.h"

Entity::Entity(std::string mNewName, SceneManager* newSceneManager)
{
	//apart from movableobject constructor
	mName = mNewName;
	mModelMatrix = glm::mat4(1.0); //identity
	mMesh = NULL;
	mMeshName = "unknown";
	mSceneManager = newSceneManager;
}

Entity::Entity(std::string mNewName, std::string meshName, SceneManager* newSceneManager)
{
	mName = mNewName;
	mModelMatrix = glm::mat4(1.0); //identity
	mSceneManager = newSceneManager;

	attachMesh(meshName); //load mesh
}


Entity::~Entity(void)
{
}


void Entity::render(glm::mat4 perspectiveViewM)
{
	//apply shader
	mSceneManager->bindCurrentShader();

	//send uniforms
	Shader* shad = mSceneManager->getCurrentShader();	
	glm::mat4 finalMatrix = perspectiveViewM * mModelMatrix; //final matrix composed of pers * view * node * model matrix
	shad->UniformMatrix("finalM", finalMatrix);
	glm::mat4 normalM = glm::inverseTranspose(mSceneManager->getViewMatrix() * mParentSceneNode->getSceneNodeMatrix() * mModelMatrix);
	shad->UniformMatrix("normalM", normalM);

	for(int i = 0; i < mMesh->mMeshComponentsVector.size(); ++i)
	{
		//render
		mMesh->bindMeshArray(mMesh->mMeshComponentsVector[i]);

		glDrawElements(GL_TRIANGLES, mMesh->mMeshComponentsVector[i].mIndexVector.size(), GL_UNSIGNED_INT, 0);

		mMesh->unbindMeshArray();

	}

	mSceneManager->unbindShader();
}

void Entity::attachMesh(std::string meshName)
{
	mMeshName = meshName;

	ResourceManager* mResourceManager = ResourceManager::getSingletonPtr(); //resourcemanager pointer
	mMesh = mResourceManager->createMesh(mMeshName, mMeshName); //allocate new mesh

	mModelMatrix = mMesh->meshMatrix;
}

void Entity::setCubeMesh()
{
	mMeshName = "cube";

	
	ResourceManager* mResourceManager = ResourceManager::getSingletonPtr(); //resourcemanager pointer
	mMesh = mResourceManager->createMesh(mMeshName, "NULL"); //allocate new mesh

	mMesh->createCube();
}