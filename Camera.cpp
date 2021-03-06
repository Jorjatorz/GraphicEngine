#include "Camera.h"

#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "Window.h"

Camera::Camera(std::string& name, SceneManager* newSceneManager)
{
	mSceneManager = newSceneManager;

	mPosition = glm::vec3(0.0);
	mOrientation = glm::vec3(0.0, 0.0, -1.0);
	mUpVector = glm::vec3(0.0, 1.0, 0.0);
	mDerivedPosition = mPosition;
	mName = name;
	mCurrentControlType = NOCONTROLER;
	movementSpeed = 2.0;
	mouseSpeed = 0.002;

	//Pointing to 0, 0, -1
	camPitch = 3.1415;
	camYaw = 0;

	mTypeOfMovableObject = tTypeEnum::CAMERA;
}

Camera::~Camera(void)
{
}

void Camera::process(glm::mat4 perspectiveViewSceneNodeM, glm::mat4 viewMatrix, glm::vec3 parentPos, glm::quat parentOrient)
{
	mDerivedPosition = parentPos;
	//Need to set the derived orientation

	updateCamera();
}

void Camera::updateCamera()
{
	updateFromInput();

	mCameraMatrix = glm::lookAt(mDerivedPosition, mOrientation + mDerivedPosition, mUpVector);
}

void Camera::updateFromInput()
{
	switch (mCurrentControlType)
	{
	default:
		break;
	case NOCONTROLER:
	{
		//Set SDL Cursor mode off (show cursor)
		SDL_ShowCursor(1);
		mDerivedPosition += mPosition;
		mUpVector = glm::vec3(0.0, 1.0, 0.0);
		break;
	}
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

void Camera::setMovementSpeed(real newSpeed)
{
	movementSpeed = newSpeed;
}

void Camera::handleMouseMove(InputManager* inputMInstance, int mouseX, int mouseY)
{
	//If we are at default control type update camera view
	if (mCurrentControlType == DEFAULT)
	{
		//center of the screen
		int centerX, centerY;
		centerX = mSceneManager->getRenderer()->getCurrentWindow()->getWidth() / 2;
		centerY = mSceneManager->getRenderer()->getCurrentWindow()->getHeight() / 2;

		//difference from current mouse pos to the center
		int diffX = centerX - mouseX;
		int diffY = centerY - mouseY;

		//calculate the new angle (radians)
		camPitch += diffX * mouseSpeed;
		camYaw += diffY * mouseSpeed;

		inputMInstance->setMousePosition(centerX, centerY);
	}
}

void Camera::transformFromInput()
{
	InputManager* inputIns = InputManager::getSingletonPtr();

	//Just reset the angles, so we prevent huge numbers
	if(camPitch > 2 * 3.1415)
		camPitch = 0;
	else if(camPitch < 0)
		camPitch = 2 * 3.1415;
	if(camYaw > 3.1415 / 2)
		camYaw = 3.1415 / 2;
	else if(camYaw < -3.1415 / 2)
		camYaw = -3.1415 / 2;

	//Calculate the new orientation
	mOrientation.x = glm::cos(camYaw) * glm::sin(camPitch);
	mOrientation.y = glm::sin(camYaw);
	mOrientation.z = glm::cos(camYaw) * glm::cos(camPitch);

	//Compute the right vector
	glm::vec3 right = glm::vec3(glm::sin(camPitch - 3.1415f / 2.0f), 0, glm::cos(camPitch - 3.1415f / 2.0f));

	//Compute the new up Vector
	mUpVector = glm::cross(right, mOrientation);
	
	//Change position depending on input
	if(inputIns->isKeyDown(SDL_SCANCODE_W))
	{
		mPosition += mOrientation * mSceneManager->mDeltaTime * movementSpeed;
	}

	if(inputIns->isKeyDown(SDL_SCANCODE_S))
	{
		mPosition -= mOrientation * mSceneManager->mDeltaTime * movementSpeed;
	}

	if(inputIns->isKeyDown(SDL_SCANCODE_D))
	{
		mPosition += right * mSceneManager->mDeltaTime * movementSpeed;
	}

	if(inputIns->isKeyDown(SDL_SCANCODE_A))
	{
		mPosition -= right * mSceneManager->mDeltaTime * movementSpeed;
	}

	//Add local position to derivd position
	mDerivedPosition += mPosition;
}