#ifndef CAMERA_H
#define CAMERA_H

#include "Definitions.h"
#include "MovableObject.h"

#include <string>

class Camera : public MovableObject
{
public:
	/**
	* @desc Sets the initial values for the camera, setting his parent sceneManager and his name
	* @param String name: the name of the Camera - SceneManager: the scenemanager that creates the camera
	* @return -
	*/
	Camera(std::string& name, SceneManager* newSceneManager);
	~Camera(void);

	//Camera control mode,
	typedef enum
	{
		NOCONTROLER, //The camera is static, does not react to input
		DEFAULT //DEfault controler, reacts to keyboard and mouse (free camera)
	} tControlerType;

	/**
	* @desc Process the camera, checking for inputs (depending on the current controler) and updates his viewMatrix
	* @param glm::mat4 perspectiveViewSceneNodeM: Useless - glm::mat4 viewMatrix: useless - glm::vec3 parentPos: the parent position of the camera - glm::quat parentOrient: the parent orientation of the camera
	* @return void
	*/
	void process(glm::mat4 perspectiveViewSceneNodeM, glm::mat4 viewMatrix, glm::vec3 parentPos, glm::quat parentOrient);

	/**
	* @desc Depending on the controler, its called by the InputManager and calculates the new pitch and yaw of the camera depending on the mouse mose.
	* @param InputManager* inputMInstance: instance of the inputmanager which called the function, int mouseX, int moyseY: mouse Coords in screen coordinates
	* @return void
	*/
	void handleMouseMove(InputManager* inputMInstance, int mouseX, int mouseY);

	/**
	* @desc Sets the current controler to the new one in the parameter
	* @param tControlerType newType: new type of controler
	* @return void
	*/
	void setControler(tControlerType newType);

	/**
	* @desc Modifies the view matrix creating a new one which looks at the new orientation
	* @param glm::vec3& newOrient: the new orientation
	* @return void
	*/
	void lookAt(glm::vec3& newOrient);

	//Sets a new Position to the camera
	void setPosition(glm::vec3& newPosition);
	//Sets a new Orientation to the camera
	void setOrientation(glm::vec3 newOrientation);
	//Sets a new MovementSpeed to the camera
	void setMovementSpeed(real newSpeed);

	//Returns the camera view matrix
	glm::mat4 getCameraMatrix()
	{
		return mCameraMatrix;
	}
	//Returns the current position of the camera
	glm::vec3 getPosition()
	{
		return mPosition;
	}
	//Returns the current orientation of the camera
	glm::vec3 getOrientation()
	{
		return mOrientation;
	}
	//Returns the current Up vector of the camera
	glm::vec3 getUpVector()
	{
		return mUpVector;
	}

private:
	//Transformation properties of the camera
	glm::vec3 mPosition, mOrientation, mUpVector;
	glm::vec3 mDerivedPosition, mDerivedOrientation;
	glm::mat4 mCameraMatrix; //view matrix (specific for each camera)

	real camYaw, camPitch; //Y rotation, X rotation

	//Control properties
	tControlerType mCurrentControlType;//Current control type attached to this camera
	real movementSpeed; //Movement speed of the camera
	real mouseSpeed; //Orientation speed of the camera

	//UpdatesFromUnput and generates a new view matrix
	void updateCamera();
	//function that generates the new position, orientation and up vector, depending on the current Controler
	void updateFromInput();
	//Calculate the new position and orientation from the user input
	void transformFromInput();

};

#endif