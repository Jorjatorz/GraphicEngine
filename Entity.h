#ifndef ENTITY_H
#define ENTITY_H

#include <string>

#include "Definitions.h"
#include "MovableObject.h"

class Entity : public MovableObject
{
public:
	Entity(std::string mNewName, SceneManager* newSceneManager);
	Entity(std::string mNewName, std::string meshName, SceneManager* newSceneManager);
	~Entity(void);

	void renderEntity();

	//types
	void setCubeMesh(); //set a cube as mesh


	glm::mat4 modelMatrix; //entity model matrix

private:
	Mesh* mMesh; //mesh pointer
	std::string mMeshName; //mesh name

};


#endif