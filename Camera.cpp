#include "Camera.h"

#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "Window.h"

Camera::Camera(SceneManager* newSceneManager)
{
	mCurrentSceneManager = newSceneManager;

	mPosition = glm::vec3(0.0);
	mOrientation = glm::vec3(0.0, 0.0, -1.0);
	mUpVector = glm::vec3(0.0, 1.0, 0.0);
	mName = "Unknown Camera";
	mCurrentControlType = NOCONTROLER;
	movementSpeed = mouseSpeed = 0.2;

	camPitch = camYaw = 0;
}

Camera::Camera(std::string& name, SceneManager* newSceneManager)
{
	mCurrentSceneManager = newSceneManager;

	mPosition = glm::vec3(0.0);
	mOrientation = glm::vec3(0.0, 0.0, -1.0);
	mUpVector = glm::vec3(0.0, 1.0, 0.0);
	mName = name;
	mCurrentControlType = NOCONTROLER;
	movementSpeed = mouseSpeed = 0.2;

	camPitch = camYaw = 0;
}

Camera::~Camera(void)
{
}

void Camera::updateCamera()
{
	updateFromInput();

	mCameraMatrix = glm::lookAt(mPosition, mOrientation + mPosition, mUpVector);
}

void Camera::updateFromInput()
{
	switch(mCurrentControlType)
	{
	default:
		break;
	case NOCONTROLER:
		//Set SDL Cursor mode off (show cursor)
		SDL_ShowCursor(1);
		mUpVector = glm::vec3(0.0, 1.0, 0.0);
		break; //no controler so we just skip
	case DEFAULT:
		{
			SDL_ShowCursor(0);
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
	//Get the new Orientation
	mOrientation = newOrient - mPosition;
	//Compute the new matrix
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

	//COMPUTE ORIENTATION
	//get the cursor position
	int mouseX, mouseY;
	inputIns->getMousePosition(mouseX, mouseY);

	//center of the screen
	int centerX, centerY;
	centerX = mCurrentSceneManager->getRenderer()->getCurrentWindow()->getWidth() / 2;
	centerY = mCurrentSceneManager->getRenderer()->getCurrentWindow()->getHeight() / 2;

	//difference from current mouse pos to the center

	int diffX = centerX - mouseX;
	int diffY = centerY - mouseY;

	//calculate the new angle (radians)
	camPitch += diffX * mCurrentSceneManager->mDeltaTime * mouseSpeed;
	camYaw += diffY * mCurrentSceneManager->mDeltaTime * mouseSpeed;

	//Just reset the angles, so we prevent huge numbers
	if(camPitch > 2 * 3.1415)
		camPitch = 0;
	else if(camPitch < 0)
		camPitch = 2 * 3.1415;
	if(camYaw > 3.1415 / 2)
		camYaw = 3.1415 / 2;
	else if(camYaw < -3.1415 / 2)
		camYaw = -3.1415 / 2;

	//Set the new orientation
	mOrientation.x = glm::cos(camYaw) * glm::sin(camPitch);
	mOrientation.y = glm::sin(camYaw);
	mOrientation.z = glm::cos(camYaw) * glm::cos(camPitch);

	//Compute the right vector
	glm::vec3 right = glm::vec3(glm::sin(camPitch - 3.1415f / 2.0f), 0, glm::cos(camPitch - 3.1415f / 2.0f));

	//Compute the new up Vector
	mUpVector = glm::cross(right, mOrientation);
	
	//COMPUTE MOVEMENT
	if(inputIns->isKeyDown(SDL_SCANCODE_W))
	{
		mPosition += mOrientation * mCurrentSceneManager->mDeltaTime * movementSpeed;
	}

	if(inputIns->isKeyDown(SDL_SCANCODE_S))
	{
		mPosition -= mOrientation * mCurrentSceneManager->mDeltaTime * movementSpeed;
	}

	if(inputIns->isKeyDown(SDL_SCANCODE_D))
	{
		mPosition += right * mCurrentSceneManager->mDeltaTime * movementSpeed;
	}

	if(inputIns->isKeyDown(SDL_SCANCODE_A))
	{
		mPosition -= right * mCurrentSceneManager->mDeltaTime * movementSpeed;
	}

	//reset mouse position
	inputIns->warpMouse(centerX, centerY);
}