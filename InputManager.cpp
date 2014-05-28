#include "InputManager.h"


InputManager::InputManager(void)
{
	//initialize array to all false
	for(int i = 0; i < SDL_NUM_SCANCODES; ++i)
	{
		mKeyPressedArray[i] = false;
	}

	for(int i = 0; i < 3; ++i)
	{
		mMousePressedArray[i] = false;
	}

	mSDLWindow = NULL;
}


InputManager::~InputManager(void)
{
}

#include "ResourceManager.h"
#include "Root.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "Window.h"
#include "Camera.h"

void InputManager::getFrameInput(bool &running)
{
	while(SDL_PollEvent(&mEvent))
	{
		if(mEvent.type == SDL_QUIT)
		{
			running = false;
			break;
		}

		if(mEvent.type == SDL_WINDOWEVENT)
		{
			if(mEvent.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
			{
				std::cout << "Focus Gained" << std::endl;
				ResourceManager* mR = ResourceManager::getSingletonPtr();
				mR->checkForModifications();
			}

		}
		
		if(mEvent.type == SDL_MOUSEMOTION)
		{
			SDL_GetMouseState(&mouseX, &mouseY);
			Camera* mCam = Root::getSingletonPtr()->getCurrentSceneManager()->getCurrentCamera();
			//Check that a camera exists
			if (mCam != NULL)
			{
				mCam->handleMouseMove(this, mouseX, mouseY);
			}
		}

		if(mEvent.type == SDL_MOUSEBUTTONDOWN)
		{
			mMousePressedArray[mEvent.button.button - 1] = true; //-1 to pass from sdl encoding to array acces encoding
		}

		if(mEvent.type == SDL_MOUSEBUTTONUP)
		{
			mMousePressedArray[mEvent.button.button - 1] = false;
		}

		if(mEvent.type == SDL_KEYDOWN)
			mKeyPressedArray[mEvent.key.keysym.scancode] = true;

		if(mEvent.type == SDL_KEYUP)
			mKeyPressedArray[mEvent.key.keysym.scancode] = false;
	}
}

bool InputManager::isKeyDown(short int keyCode)
{
	return mKeyPressedArray[keyCode];
}

bool InputManager::isKeyUp(short int keyCode)
{
	//return the array negated because if the key is not pressed (false) then its up (so !false = true)
	return !mKeyPressedArray[keyCode];
}

bool InputManager::isMouseButtonDown(short int buttonCode)
{
	return mMousePressedArray[buttonCode - 1]; //transform from 1, 2, 3 sdl numbers into 0, 1, 2 array acces numbers
}
bool InputManager::isMouseButtonUp(short int buttonCode)
{
	return !mMousePressedArray[buttonCode - 1];
}

void InputManager::setMousePosition(int x, int y)
{
	SDL_WarpMouseInWindow(mSDLWindow, x, y);
}

void InputManager::getMousePosition(int &x, int &y)
{
	x = mouseX;
	y = mouseY;
}