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

	void attachMesh(std::string meshName);

	void render(glm::mat4 perspectiveViewM);

	//types
	void setCubeMesh(); //set a cube as mesh

private:
	Mesh* mMesh; //mesh pointer
	std::string mMeshName; //mesh name
	glm::mat4 mModelMatrix; //entity model matrix

};


#endif