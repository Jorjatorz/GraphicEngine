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

	void updateCamera();

	void lookAt(glm::vec3& newOrient);

	void setPosition(glm::vec3& newPosition);
	void setOrientation(glm::vec3 newOrientation);

	glm::mat4 getCameraMatrix()
	{
		return mCameraMatrix;
	}

<<<<<<< HEAD
=======
	glm::vec3 getPosition()
	{
		return mPosition;
	}

	glm::vec3 setOrientation()
	{
		return mOrientation;
	}
>>>>>>> f60680574ae3ccb0f9be17c78d310144eda8d124
	std::string getName()
	{
		return mName;
	}

private:
	glm::vec3 mPosition, mOrientation;
	std::string mName; //camera name

	glm::mat4 mCameraMatrix; //view matrix (specific for each camera)
};

#endif