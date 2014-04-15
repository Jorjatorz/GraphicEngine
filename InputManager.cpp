#include "InputManager.h"


InputManager::InputManager(void)
{
	//initialize array to all false
	for(int i = 0; i < SDL_NUM_SCANCODES; ++i)
	{
		mKeyPressedArray[i] = false;
	}
}


InputManager::~InputManager(void)
{
}

#include "ResourceManager.h"

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