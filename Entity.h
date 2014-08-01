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

	void process(glm::mat4 perspectiveViewSceneNodeM, glm::mat4 viewMatrix, glm::vec3 parentPos = glm::vec3(0.0), glm::vec3 parentOrient = glm::vec3(0.0)); //inheritated

	void attachMesh(std::string meshName);
	void deAttachMesh();

	void attachMaterial(std::string materialName);

	void setWireFrame(bool wire);
	void setModelMatrix(glm::mat4 matrix)
	{
		mModelMatrix = matrix;
	}

	Material* getMaterial()
	{
		return mMaterial;
	}

	glm::mat4 getModelMatrix()
	{
		return mModelMatrix;
	}

private:
	Mesh* mMesh; //mesh pointer
	Material* mMaterial; //Material attached to this entity
	bool meshAttached; //is any mesh attached?

	std::string mMeshName; //mesh name
	glm::mat4 mModelMatrix; //entity model matrix

	void render(glm::mat4 perspectiveViewSceneNodeM, glm::mat4 viewMatrix);
	void sendEntityUniforms(Shader* currentShader, glm::mat4 PVNMatrix, glm::mat4 viewMatrix);

	bool wireFrame;

	/*
	//inheritated
	std::string mName;
	SceneManager* mSceneManager;
	SceneNode* mParentSceneNode;
	bool visible;
	bool castShadows;
	bool mAttachedToNode; //is attached to a scenode?*/
};


#endif