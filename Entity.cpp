#include "Entity.h"


#include "Mesh.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "SceneManager.h"

Entity::Entity(SceneManager* newSceneManager)
{
	modelMatrix = glm::mat4(); //identity
	mMesh = NULL;
	mMeshName = "unknown";
	mSceneManager = newSceneManager;
}

Entity::Entity(std::string meshName, SceneManager* newSceneManager)
{
	modelMatrix = glm::mat4(); //identity
	mMesh = NULL;
	mMeshName = meshName;
	mSceneManager = newSceneManager;
}


Entity::~Entity(void)
{
}


void Entity::renderEntity()
{
	//apply shader
	mSceneManager->bindCurrentShader();

	//render
	mMesh->bindMeshArray();

	//send uniforms

	glDrawElements(GL_TRIANGLES, mMesh->numberOfVertices, GL_UNSIGNED_SHORT, 0);

	mMesh->unbindMeshArray();

	mSceneManager->unbindCurrentShader();
}

void Entity::setCubeMesh()
{
	mMeshName = "cube";

	ResourceManager* mResourceManager = ResourceManager::getSingletonPtr(); //resourcemanager pointer
	mMesh = mResourceManager->createMesh(mMeshName, "NULL"); //allocate new mesh

	mMesh->createCube();
}