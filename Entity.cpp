#include "Entity.h"


#include "Mesh.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "SceneManager.h"

Entity::Entity(std::string mNewName, SceneManager* newSceneManager)
{
	//apart from movableobject constructor
	mName = mNewName;
	modelMatrix = glm::mat4(1.0); //identity
	mMesh = NULL;
	mMeshName = "unknown";
	mSceneManager = newSceneManager;
}

Entity::Entity(std::string mNewName, std::string meshName, SceneManager* newSceneManager)
{
	mName = mNewName;
	modelMatrix = glm::mat4(1.0); //identity
	mMesh = NULL;
	mMeshName = meshName;
	mSceneManager = newSceneManager;
}


Entity::~Entity(void)
{
}


void Entity::render(glm::mat4 perspectiveViewM)
{
	//apply shader
	mSceneManager->bindCurrentShader();

	Shader* shad = mSceneManager->getCurrentShader();
	
	glm::mat4 finalMatrix = perspectiveViewM * modelMatrix; //final matrix composed of pers * view * node * model matrix
	shad->UniformMatrix("finalM", finalMatrix);

	//render
	mMesh->bindMeshArray();

	//send uniforms

	glDrawElements(GL_TRIANGLES, mMesh->numberOfVertices, GL_UNSIGNED_SHORT, 0);

	mMesh->unbindMeshArray();

	mSceneManager->unbindShader();
}

void Entity::setCubeMesh()
{
	mMeshName = "cube";

	ResourceManager* mResourceManager = ResourceManager::getSingletonPtr(); //resourcemanager pointer
	mMesh = mResourceManager->createMesh(mMeshName, "NULL"); //allocate new mesh

	mMesh->createCube();
}