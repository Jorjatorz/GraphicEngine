#ifndef ENTITY_H
#define ENTITY_H

#include <string>

#include "Definitions.h"


class Entity
{
public:
	Entity(void);
	Entity(std::string meshName);
	~Entity(void);

	void renderEntity(Shader* currentShader);

	//types
	void setCubeMesh(); //set a cube as mesh


	glm::mat4 modelMatrix; //entity model matrix

private:
	Mesh* mMesh; //mesh pointer
	std::string mMeshName; //mesh name

};


#endif