#include "Entity.h"


#include "Mesh.h"
#include "Shader.h"
#include "ResourceManager.h"

Entity::Entity(void)
{
	modelMatrix = glm::mat4(); //identity
	mMesh = NULL;
	mMeshName = "unknown";
}

Entity::Entity(std::string meshName)
{
	modelMatrix = glm::mat4(); //identity
	mMesh = NULL;
	mMeshName = meshName;
}


Entity::~Entity(void)
{
}


void Entity::renderEntity(Shader* currentShader)
{
	//apply shader
	currentShader->bind();

	//render
	mMesh->bindMeshArray();

	//send uniforms

	glDrawElements(GL_TRIANGLES, mMesh->numberOfVertices, GL_UNSIGNED_SHORT, 0);

	mMesh->unbindMeshArray();

	currentShader->unBind();
}

void Entity::setCubeMesh()
{
	mMeshName = "cube";

	ResourceManager* mResourceManager = ResourceManager::getSingletonPtr(); //resourcemanager pointer
	mMesh = mResourceManager->createMesh(mMeshName, "NULL"); //allocate new mesh

	mMesh->createCube();
}