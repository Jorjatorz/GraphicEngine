#ifndef ENTITY_H
#define ENTITY_H

#include <string>

#include "Definitions.h"
#include "MovableObject.h"

class Entity : public MovableObject
{
public:
	//Constructors, creates a new entity and save the sceneMasnager that creates it. Also, if not NULL, load the mesh 
	Entity(std::string mNewName, SceneManager* newSceneManager);
	Entity(std::string mNewName, std::string meshName, SceneManager* newSceneManager);
	~Entity(void);

	//Renders the entity to the framebuffer
	void process(glm::mat4 perspectiveViewSceneNodeM, glm::mat4 viewMatrix, glm::vec3 parentPos = glm::vec3(0.0), glm::quat parentOrient = glm::quat()); //inheritated

	//Directly from entity - It returns the scenenode derived position (Can be also done directly from the Scenenode where it's attached)
	glm::vec3 getPosition();
	glm::vec3 getOrientation_Euler();
	glm::vec3 getScale();

	//Mesh part---------------------------------------------------
	//Attach a new Mesh and asks the resource manager to load it
	void attachMesh(std::string meshName);
	//Sets the currentMesh of the entity to NULL
	void deAttachMesh();
	Mesh* getMesh()
	{
		return mMesh;
	}

	//Physics part-----------------------------------------------
	//Creates a rigidBody when the entity is attached to a SceneNode (because this process need the position, scale and orientation of the entity). Its deleted by the destructor
	void makeRigidBody(SceneNode* node);
	//void makeCollisionObject(SceneNode* node);
	//Only if kinetic object! Updates the position, scale and orientation of the Rigidbody
	void setRigidBodyTransforms(SceneNode* node);
	RigidBody* getRigidBody()
	{
		return mRigidBody;
	}
	//Returns the mass of the RigidBody
	real getMass();
	//Sets the mass of the RigidBody (thus it change the properties of it) -> if != 0 -> Dynamic | if == 0 and setStatic -> Static else Kinetic
	void setMass(real mass, bool setStatic);
	//Set physics interaction enabled or disabled
	void setPhysicsOn(bool mod);
	//Sets the pointer that is returned when a rayCast hits the entity Rigidbody
	void setRayCastReturnPointer(void *returnPointer);

	//Material part----------------------------------------------
	//Attachs a material to the entity, if it doesnt exists it's created.
	void attachMaterial(std::string materialName);
	//If true the entity will be rendered as a wireframe
	void setWireFrame(bool wire);
	//Shortcut to the material function setBaseColorV. Sets the new base color of the material
	void setColor(glm::vec3 col);
	Material* getMaterial()
	{
		return mMaterial;
	}

	//Others----------------------------------------------------
	//Sets a new modelMatrix to the entity (Default: Identity)
	void setModelMatrix(glm::mat4 matrix);
	glm::mat4 getModelMatrix()
	{
		return mModelMatrix;
	}

private:
	//Mesh name
	std::string mMeshName;
	//Pointer to the entity mesh
	Mesh* mMesh;
	//True if it has a mesh attached
	bool meshAttached;

	//Pointer to the material attached to this entity
	Material* mMaterial;
	//True if its rendered as a wireframe
	bool wireFrame;

	//Pointer to the Rigidbody
	RigidBody* mRigidBody;
	//True if the entity is affected by physics (Default: Yes)
	bool mAffectedByPhysics;

	//Entity model matrix
	glm::mat4 mModelMatrix; 

	//Renders the mesh to the framebuffer
	void render(glm::mat4 perspectiveViewSceneNodeM, glm::mat4 viewMatrix);
	//Sends all the uniforms to the shader
	void sendEntityUniforms(Shader* currentShader, glm::mat4 PVNMatrix, glm::mat4 viewMatrix);
};


#endif