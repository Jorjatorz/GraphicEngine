#include "Camera.h"

#include "InputManager.h"

Camera::Camera(void)
{
	mPosition = glm::vec3(0.0);
	mOrientation = glm::vec3(0.0, 0.0, -1.0);
	mName = "Unknown Camera";
	mCurrentControlType = NOCONTROLER;
	movementSpeed = mouseSpeed = 1.0;
}

Camera::Camera(std::string& name)
{
	mPosition = glm::vec3(0.0);
	mOrientation = glm::vec3(0.0, 0.0, -1.0);
	mName = name;
	mCurrentControlType = NOCONTROLER;
	movementSpeed = mouseSpeed = 1.0;
}

Camera::~Camera(void)
{
}

void Camera::updateCamera()
{
	updateFromInput();

	mCameraMatrix = glm::lookAt(mPosition, mOrientation, glm::vec3(0.0, 1.0, 0.0));
}

void Camera::updateFromInput()
{
	switch(mCurrentControlType)
	{
	default:
		break;
	case NOCONTROLER:
		//Set SDL Cursor mode off (show cursor)cccc
		SDL_SetRelativeMouseMode(SDL_FALSE);
		break; //no controler so we just skip
	case DEFAULT:
		{
			transformFromInput();
			break;
		}
	}
}

void Camera::setControler(tControlerType newType)
{
	mCurrentControlType = newType;
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

void Camera::transformFromInput()
{
	InputManager* inputIns = InputManager::getSingletonPtr();

	//Set SDL Cursor mode on (hide and get mouse input per frame)
	SDL_SetRelativeMouseMode(SDL_TRUE);

	int mouseX, mouseY;
	//get the cursor position
	inputIns->getMousePosition(mouseX, mouseY);
}