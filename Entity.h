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

	void process(glm::mat4 perspectiveViewSceneNodeM, glm::mat4 viewMatrix, glm::vec3 parentPos = glm::vec3(0.0), glm::quat parentOrient = glm::quat()); //inheritated

	//Directly from entity - It returns the scenenode derived position
	glm::vec3 getPosition();
	glm::vec3 getOrientation_Euler();
	glm::vec3 getScale();

	//Mesh
	void attachMesh(std::string meshName);
	void deAttachMesh();
	Mesh* getMesh()
	{
		return mMesh;
	}

	//Physics
	void makeRigidBody(SceneNode* node);
	//void makeCollisionObject(SceneNode* node);
	void setRigidBodyTransforms(SceneNode* node);
	RigidBody* getRigidBody()
	{
		return mRigidBody;
	}
	real getMass(); //From rigidbody
	void setMass(real mass, bool setStatic);
	void setPhysicsOn(bool mod);
	void setRayCastReturnPointer(void *returnPointer);

	/*CollisionObject* getCollisionObject()
	{
		return mCollisionObject;
	}*/

	//Material
	void attachMaterial(std::string materialName);
	void setWireFrame(bool wire);
	void setColor(glm::vec3 col); //Shortcut to set the base color to the mat

	//Others
	void setModelMatrix(glm::mat4 matrix);

	Material* getMaterial()
	{
		return mMaterial;
	}

	glm::mat4 getModelMatrix()
	{
		return mModelMatrix;
	}
	SceneNode* getAttachedSceneNode()
	{
		assert(mParentSceneNode != NULL);

		return mParentSceneNode;
	}

private:
	Mesh* mMesh; //mesh pointer
	Material* mMaterial; //Material attached to this entity
	//Physics
	RigidBody* mRigidBody; //RigidBody pointer
	bool mAffectedByPhysics;

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