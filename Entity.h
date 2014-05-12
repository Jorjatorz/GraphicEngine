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

	void process(glm::mat4 perspectiveViewSceneNodeM, glm::mat4 viewMatrix, glm::vec3 parentPos, glm::vec3 parentOrient); //inheritated

	void attachMesh(std::string meshName);
	void deAttachMesh();

	void attachMaterial(std::string materialName);

	Material* getMaterial()
	{
		return mMaterial;
	}

private:
	Mesh* mMesh; //mesh pointer
	Material* mMaterial; //Material attached to this entity
	bool meshAttached; //is any mesh attached?

	std::string mMeshName; //mesh name
	glm::mat4 mModelMatrix; //entity model matrix

	void render(glm::mat4 perspectiveViewSceneNodeM);

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