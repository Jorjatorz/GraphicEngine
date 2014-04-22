#ifndef CAMERA_H
#define CAMERA_H

#include "Definitions.h"

#include <string>

class Camera
{
public:
	Camera(void);
	Camera(std::string& name);
	~Camera(void);

	//camera control mode
	typedef enum
	{
		NOCONTROLER,
		DEFAULT
	} tControlerType;


	void updateCamera();

	void setControler(tControlerType newType);

	void lookAt(glm::vec3& newOrient);

	void setPosition(glm::vec3& newPosition);
	void setOrientation(glm::vec3 newOrientation);

	glm::mat4 getCameraMatrix()
	{
		return mCameraMatrix;
	}

	glm::vec3 getPosition()
	{
		return mPosition;
	}

	glm::vec3 setOrientation()
	{
		return mOrientation;
	}
	std::string getName()
	{
		return mName;
	}

private:
	//standar variables
	glm::vec3 mPosition, mOrientation;
	std::string mName; //camera name
	glm::mat4 mCameraMatrix; //view matrix (specific for each camera)

	//Special variables
	tControlerType mCurrentControlType; //current control mode

	//Control units variables
	real movementSpeed;
	real mouseSpeed;

	//FUNCTIONS
	void updateFromInput(); //function that updates the camera view depending on the current Controler
	void transformFromInput(); //calculate the new position and orientation depending on the user input
};

#endif