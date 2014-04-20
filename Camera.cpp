#include "Camera.h"


Camera::Camera(void)
{
	mPosition = glm::vec3(0.0);
	mOrientation = glm::vec3(0.0, 0.0, -1.0);
	mName = "Unknown Camera";
}

Camera::Camera(std::string& name)
{
	mPosition = glm::vec3(0.0);
	mOrientation = glm::vec3(0.0, 0.0, -1.0);
	mName = name;
}

Camera::~Camera(void)
{
}

void Camera::updateCamera()
{
	mCameraMatrix = glm::lookAt(mPosition, mOrientation, glm::vec3(0.0, 1.0, 0.0));
}

void Camera::lookAt(glm::vec3& newOrient)
{
	mOrientation = newOrient;

	mCameraMatrix = glm::lookAt(mPosition, mOrientation, glm::vec3(0.0, 1.0, 0.0));
}

void Camera::setPosition(glm::vec3& newPosition)
{
	mPosition = newPosition;
}
void Camera::setOrientation(glm::vec3 newOrientation)
{
	mOrientation = newOrientation;
}