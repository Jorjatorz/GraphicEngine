#ifndef CAMERA_H
#define CAMERA_H

#include "Definitions.h"
#include "MovableObject.h"

#include <string>

class Camera : public MovableObject
{
public:
	Camera(SceneManager* newSceneManager);
	Camera(std::string& name, SceneManager* newSceneManager);
	~Camera(void);

	//camera control mode
	typedef enum
	{
		NOCONTROLER,
		DEFAULT
	} tControlerType;

	void process(glm::mat4 perspectiveViewSceneNodeM, glm::mat4 viewMatrix, glm::vec3 parentPos, glm::vec3 parentOrient); //inheritated

	void handleMouseMove(InputManager* inputMInstance, int x, int y);


	void setControler(tControlerType newType);

	void lookAt(glm::vec3& newOrient);

	void setPosition(glm::vec3& newPosition);
	void setOrientation(glm::vec3 newOrientation);
	void setMovementSpeed(real newSpeed);


	glm::mat4 getCameraMatrix()
	{
		return mCameraMatrix;
	}

	glm::vec3 getPosition()
	{
		return mPosition;
	}
	glm::vec3 getOrientation()
	{
		return mOrientation;
	}
	std::string getName()
	{
		return mName;
	}

private:
	//standar variables
	glm::vec3 mPosition, mOrientation, mUpVector;
	glm::vec3 mDerivedPosition, mDerivedOrientation;
	glm::mat4 mCameraMatrix; //view matrix (specific for each camera)

	real camYaw, camPitch; //Y rotation, X rotation

	//Special variables
	tControlerType mCurrentControlType; //current control mode

	//Control units variables
	real movementSpeed;
	real mouseSpeed;

	//FUNCTIONS
	void updateCamera();
	void updateFromInput(); //function that updates the camera view depending on the current Controler
	void transformFromInput(); //calculate the new position and orientation depending on the user input

	/*
	//inherited
	std::string mName;
	SceneManager* mSceneManager;
	SceneNode* mParentSceneNode;
	bool visible;
	bool castShadows;
	bool mAttachedToNode; //is attached to a scenode?*/
};

#endif